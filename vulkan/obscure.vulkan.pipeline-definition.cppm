module;
#include <array>
#include <type_traits>
#include <vulkan/vulkan.hpp>
export module obscure.vulkan.pipeline:definition;
import :draw_call_base;
import :builder;

export namespace obscure::vulkan
{
    template<typename T>
    concept pipeline_definition =
        requires {
            typename T::shader_list;
            { T::shader_list::size() } -> std::convertible_to<std::size_t>;

            typename T::draw_calls;
            requires std::is_base_of_v<draw_call_base, typename T::draw_calls>;
        } && requires(vk::Device d,
                      vk::RenderPass r,
                      vk::SampleCountFlagBits samples,
                      std::array<vk::ShaderModule, T::shader_list::size()> s) {
            { T::initialize(d, r, samples, s) } -> pipeline_builder;
        };

    template<pipeline_definition Pipeline>
    using builder_t = typename std::invoke_result_t<typename Pipeline::initialize,
                                                    vk::Device,
                                                    vk::RenderPass,
                                                    std::array<vk::ShaderModule, Pipeline::shader_list::size()>>;
} // namespace obscure::vulkan