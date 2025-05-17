#include <GLFW/glfw3.h>

import obscure;


int main()
{
	obscure::initialize("Test App", obscure::version{1,0,0});


	{
		obscure::graphics_context<obscure::builtin::pipeline::color_2d> app{};

		auto vertex_buffer1 = app.init_vertex_buffer<obscure::builtin::pipeline::color_2d_vertex>({
			{{0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
			{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
			{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
			{{0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
			{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
			{{-0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
			{{0.4f, -0.4f}, {1.0f, 1.0f, 1.0f}},
			{{0.4f, 0.4f}, {1.0f, 1.0f, 1.0f}},
			{{-0.4f, 0.4f}, {1.0f, 1.0f, 1.0f}},
			{{0.4f, -0.4f}, {1.0f, 1.0f, 1.0f}},
			{{-0.4f, 0.4f}, {1.0f, 1.0f, 1.0f}},
			{{-0.4f, -0.4f}, {1.0f, 1.0f, 1.0f}}
		});

		while (!app.window.should_close())
		{
			glfwPollEvents();
			if (app.window.isKeyPressed(GLFW_KEY_ESCAPE))
			{
				break;
			}
			{
				auto frame = app.begin_frame();
				frame.draw_color_2d(vertex_buffer1, 12);

			}
			app.submit_frame();
			app.draw_frame();
		}
	}
	obscure::free_instance();

	return 0;
}
