#include "obscure/vulkan/builtin_pipelines/test_triangle.hpp"
#include "obscure/application.hpp"


int main()
{

	obscure::application<obscure::vulkan::builtin_pipelines::test_triangle> app{"Test App", obscure::version{1,0,0}};

	{

		while (!app.window.should_close())
		{
			glfwPollEvents();
			if (app.window.isKeyPressed(GLFW_KEY_ESCAPE))
			{
				break;
			}
			{
				auto frame = app.begin_frame();
				frame.test_triangle();
			}
		}
	}
	

	return 0;
}