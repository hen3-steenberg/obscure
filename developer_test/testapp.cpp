#include <GLFW/glfw3.h>

import obscure;


int main()
{
	obscure::initialize("Test App", obscure::version{1,0,0});


	{
		obscure::graphics_context<obscure::builtin::pipeline::test_triangle> app{};

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
	
	obscure::free_instance();

	return 0;
}
