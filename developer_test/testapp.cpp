#include "obscure/vulkan/builtin_shaders.hpp"
#include "obscure/application.hpp"
#include "obscure/utils/key_map.hpp"
#include "obscure/vulkan/builtin_pipelines/test_triangle.hpp"


int main()
{

	obscure::application app{"Test App", obscure::version{1,0,0}};

	{
		using shader_list = obscure::key_map<obscure::vulkan::shader_module, obscure::vulkan::builtin_shader::test_triangle_fragment, obscure::vulkan::builtin_shader::test_triangle_vertex>;
		shader_list shaders{ app.vk_device.get() };

		auto shader_stage_crate_infos = obscure::vulkan::get_builtin_shader_stage_infos<vk::ShaderStageFlagBits::eVertex, vk::ShaderStageFlagBits::eFragment>(
			shaders.get_value<obscure::vulkan::builtin_shader::test_triangle_vertex>(),
			shaders.get_value<obscure::vulkan::builtin_shader::test_triangle_fragment>()
		);

		while (!app.window.should_close())
		{
			glfwPollEvents();
			if (app.window.isKeyPressed(GLFW_KEY_ESCAPE))
			{
				break;
			}
		}

		shaders.get_value<obscure::vulkan::builtin_shader::test_triangle_fragment>().free(app.vk_device);
		shaders.get_value<obscure::vulkan::builtin_shader::test_triangle_vertex>().free(app.vk_device);
	}
	

	return 0;
}