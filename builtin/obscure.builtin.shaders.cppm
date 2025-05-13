module;
#include <span>
#include <cstdint>
export module obscure.builtin.shaders;
export import obscure.vulkan.shader;

export namespace obscure::builtin
{
    enum class shader
    {
        no_shader,
        color_fragment,
        test_triangle_vertex,
    };

    template<shader shader>
    std::span<const uint32_t> get_data() = delete;

    template<>
    std::span<const uint32_t> get_data<shader::color_fragment>();

    template<>
    std::span<const uint32_t> get_data<shader::test_triangle_vertex>();
}

export
template<obscure::builtin::shader shader>
struct shader_loader<shader> {
    static obscure::vulkan::shader_data auto load_shader() {
        return obscure::builtin::get_data<shader>();
    }
};
