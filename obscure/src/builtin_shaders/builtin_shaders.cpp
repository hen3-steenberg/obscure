#include "obscure/vulkan/builtin_shaders.hpp"

using namespace obscure::vulkan;

constexpr const unsigned char test_triangle_frag[] = {
#embed "test_triangle.frag.spv"
,0, 0, 0, 0
};

constexpr const unsigned char test_triangle_vert[] = {
#embed "test_triangle.vert.spv"
,0, 0 , 0, 0
};

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

template<builtin_shader shader>
size_t get_builtin_size() = delete;

template<>
size_t get_builtin_size<builtin_shader::test_triangle_fragment>()
{
	constexpr size_t size = sizeof(test_triangle_frag);
	return get_actual_size(size);
}

template<>
size_t get_builtin_size<builtin_shader::test_triangle_vertex>()
{
	constexpr size_t size = sizeof(test_triangle_vert);
	return get_actual_size(size);
}


template<>
std::span<const uint32_t>  obscure::vulkan::get_data<builtin_shader::test_triangle_fragment>()
{
	static auto data = reinterpret_cast<const uint32_t*>(test_triangle_frag);
	return std::span<const uint32_t> {data, get_builtin_size<builtin_shader::test_triangle_fragment>()};
}

template<>
std::span<const uint32_t> obscure::vulkan::get_data<builtin_shader::test_triangle_vertex>()
{
	static auto data = reinterpret_cast<const uint32_t*>(test_triangle_vert);
	return std::span<const uint32_t> {data, get_builtin_size<builtin_shader::test_triangle_vertex>()};
}



