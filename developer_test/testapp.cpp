#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#include <ratio>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
			{{-0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}}
		});

		auto vertex_buffer2 = app.init_vertex_buffer<obscure::builtin::pipeline::color_2d_vertex>({
			{{0.4f, -0.4f}, {1.0f, 1.0f, 1.0f}},
			{{0.4f, 0.4f}, {1.0f, 1.0f, 1.0f}},
			{{-0.4f, 0.4f}, {1.0f, 1.0f, 1.0f}},
			{{-0.4f, -0.4f}, {1.0f, 1.0f, 1.0f}}
		});

		auto rectangle_indices = app.init_index_buffer<uint16_t>({0, 1, 2, 2, 3, 0});

		auto uniform = app.create_uniform<obscure::builtin::pipeline::color_2d_uniform, obscure::builtin::pipeline::color_2d>(0);

		obscure::stopwatch<float> frame_timer{};
		while (!app.window.should_close())
		{
			glfwPollEvents();
			if (app.window.isKeyPressed(GLFW_KEY_ESCAPE))
			{
				break;
			}
			{
				auto frame = app.begin_frame();
				uniform.set_current_index(frame.get_frame_index());
				auto extent = frame.get_extent();

				float time = frame_timer.total_time().count();
				uniform->model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
				uniform->view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
				uniform->proj = glm::perspective(glm::radians(45.0f), extent.width / (float) extent.height, 0.1f, 10.0f);
				uniform->proj[1][1]*= -1;

				frame.draw_color_2d(uniform, vertex_buffer1, rectangle_indices);
				frame.draw_color_2d(uniform, vertex_buffer2, rectangle_indices);

			}
			app.submit_frame();
			app.draw_frame();
		}
	}
	obscure::free_instance();

	return 0;
}
