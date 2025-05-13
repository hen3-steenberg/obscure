module;
#include <span>
#include <cstdint>
module obscure.builtin.shaders;
using namespace obscure::builtin;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc23-extensions"
constexpr const unsigned char color_frag[] = {
#embed "color.frag.spv"
    ,0, 0, 0, 0
    };

constexpr const unsigned char test_triangle_vert[] = {
#embed "test_triangle.vert.spv"
    ,0, 0 , 0, 0
    };
#pragma clang diagnostic pop

constexpr size_t get_actual_size(size_t size)
{
    if (size % 4)
    {
        return size / 4;
    }
    else
    {
        return (size - 4) / 4;
    }
}

template<shader shader>
size_t get_builtin_size() = delete;

template<>
size_t get_builtin_size<shader::color_fragment>()
{
    constexpr size_t size = sizeof(color_frag);
    return get_actual_size(size);
}

template<>
size_t get_builtin_size<shader::test_triangle_vertex>()
{
    constexpr size_t size = sizeof(test_triangle_vert);
    return get_actual_size(size);
}


template<>
std::span<const uint32_t>  obscure::builtin::get_data<shader::color_fragment>()
{
    static auto data = reinterpret_cast<const uint32_t*>(color_frag);
    return std::span<const uint32_t> {data, get_builtin_size<shader::color_fragment>()};
}

template<>
std::span<const uint32_t> obscure::builtin::get_data<shader::test_triangle_vertex>()
{
    static auto data = reinterpret_cast<const uint32_t*>(test_triangle_vert);
    return std::span<const uint32_t> {data, get_builtin_size<shader::test_triangle_vertex>()};
}