#include "obscure/vulkan/builtin_pipelines/test_triangle.hpp"
#include "obscure/application.hpp"
#include "obscure/context.hpp"


int main()
{
	obscure::initialize("Test App", obscure::version{1,0,0});

	obscure::application<obscure::vulkan::builtin_pipelines::test_triangle> app{};

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
				frame.draw_test_triangle();
			}
			app.submit_frame();
			app.draw_frame();
		}
	}
	

	return 0;
}