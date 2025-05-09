module;
#include <array>
#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#include <vulkan/vulkan.hpp>
export module obscure.vulkan.shader:set;
export import obscure.utils.key_set;
import :modules;
import obscure.vulkan.pipeline;

export namespace obscure::vulkan
{
    template<auto ... Keys>
    struct shader_set : private vk::Device {
    private:
        [[nodiscard]] vk::Device get_device() const
        {
            return static_cast<vk::Device>(*this);
        }

    public:
        using shader_list = obscure::key_set<Keys...>;
        std::array<shader_module, sizeof...(Keys)> shaders;

        explicit shader_set(vk::Device device)
            : vk::Device(device), shaders{ shader_module::load_shader<Keys>(device)... }
        {}

        ~shader_set()
        {
            for (auto& shader : shaders)
            {
                shader.free(get_device());
            }
        }

        template<auto ... SubKeys>
        auto get_sub_set() const
        {
            return std::array<vk::ShaderModule, sizeof...(SubKeys)> {
                static_cast<vk::ShaderModule>(shaders[shader_list::template get_index<SubKeys>()])...
            };
        }

        template<auto ... SubKeys>
        auto get_shaders(obscure::key_set<SubKeys...> obj) const
        {
            return get_sub_set<SubKeys...>();
        }
    };

    template<typename... Types>
    constexpr bool any() {
        return sizeof...(Types) > 0;
    }

    template<pipeline_definition ... TPipelines>
    struct make_shader_list;

    template<pipeline_definition TFirst, pipeline_definition ... TPipelines>
    struct make_shader_list<TFirst, TPipelines...>
    {
        using shader_list = typename make_shader_list<TPipelines...>::type;
        using type = typename TFirst::shader_list:: template add_key_list<shader_list>;
    };

    template<>
    struct make_shader_list<>
    {
        using type = obscure::key_set<>;
    };

    template<pipeline_definition ... TPipelines>
    using make_shader_list_t = typename make_shader_list<TPipelines...>::type;

    template<typename TList>
    struct make_shader_set
    {};

    template<auto ... Keys>
    struct make_shader_set<obscure::key_set<Keys...>>
    {
        using type = shader_set<Keys...>;
    };

    template<pipeline_definition ... TPipelines>
    using make_shader_set_t = typename make_shader_set<make_shader_list_t<TPipelines...>>::type;
}