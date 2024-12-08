#ifndef OBSCURE_VULKAN_BUILTIN_PIPELINE_TEST_TRIANGLE_DEFINITION
#define OBSCURE_VULKAN_BUILTIN_PIPELINE_TEST_TRIANGLE_DEFINITION 1
#include "obscure/utils/key_set.hpp"
#include "obscure/vulkan/builtin_shaders.hpp"
#include "obscure/vulkan/pipeline_builder.hpp"
#include <array>

namespace obscure
{
	namespace vulkan
	{
		namespace builtin_pipelines
		{
			struct test_triangle
			{
				using shader_list = obscure::make_set<obscure::vulkan::builtin_shader::test_triangle_fragment, obscure::vulkan::builtin_shader::test_triangle_fragment>;

				static pipeline_builder<2, 2> initialize(std::array<vk::ShaderModule, 2> const& shaders)
				{

				}

			};
		}
	}
}
#endif