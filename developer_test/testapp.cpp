#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#include <ratio>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vulkan/vulkan.hpp>
#include <filesystem>

import obscure;


int main()
{
	obscure::initialize("Test App", obscure::version{1,0,0});


	{
		obscure::graphics_context<obscure::builtin::pipeline::color_2d, obscure::builtin::pipeline::texture_2d> app{};

		auto vertex_buffer1 = app.init_vertex_buffer<obscure::builtin::pipeline::color_2d_vertex>({
			{{0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
			{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
			{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
			{{-0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}}
		});

		auto vertex_buffer2 = app.init_vertex_buffer<obscure::builtin::pipeline::texture_2d_vertex>({
			{{0.4f, -0.4f}, {1.0f, 0.0f}},
			{{0.4f, 0.4f}, {0.0f, 0.0f}},
			{{-0.4f, 0.4f}, {0.0f, 1.0f}},
			{{-0.4f, -0.4f}, {1.0f, 1.0f}}
		});

		auto rectangle_indices = app.init_index_buffer<uint16_t>({0, 1, 2, 2, 3, 0});

		glm::mat4 view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

		auto texture = app.load_texture<obscure::builtin::pipeline::texture_2d, vk::SamplerAddressMode::eRepeat> ("./texture.jpg", 0);


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
				auto extent = frame.get_extent();

				float time = frame_timer.total_time().count();
				glm::mat4 model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
				glm::mat4 proj = glm::perspective(glm::radians(45.0f), extent.width / (float) extent.height, 0.1f, 10.0f);
				proj[1][1]*= -1;

				glm::mat4 viewproj = proj * view;

				frame.draw_color_2d(viewproj, model, vertex_buffer1, rectangle_indices);
				frame.draw_texture_2d(viewproj, model, vertex_buffer2, rectangle_indices, texture);

			}
			app.submit_frame();
			app.draw_frame();
		}
	}
	obscure::free_instance();

	return 0;
}
