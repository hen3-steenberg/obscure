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

		template<builtin_shader shader>
		std::span<const uint32_t> get_data() = delete;

		template<>
		std::span<const uint32_t> get_data<builtin_shader::test_triangle_fragment>();

		template<>
		std::span<const uint32_t> get_data<builtin_shader::test_triangle_vertex>();
	}
}

template<obscure::vulkan::builtin_shader shader>
obscure::vulkan::shader_data auto get_shader_data() {
	return obscure::vulkan::get_data<shader>();
}

#endif