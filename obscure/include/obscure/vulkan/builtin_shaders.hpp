#ifndef OBSCURE_VULKAN_BUILTIN_SHADER_DEFINITION
#define OBSCURE_VULKAN_BUILTIN_SHADER_DEFINITION 1
#include "obscure/vulkan/shader_data.hpp"

namespace obscure
{
	namespace vulkan
	{
		enum class builtin_shader
		{
			no_shader,
			test_triangle_fragment,
			test_triangle_vertex,
		};
	}
}

template<>
std::span<const uint32_t> get_shader_data<obscure::vulkan::builtin_shader::test_triangle_fragment>();

template<>
std::span<const uint32_t> get_shader_data<obscure::vulkan::builtin_shader::test_triangle_vertex>();

#endif