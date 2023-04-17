#include "obscure/application_2d.h"
#include "obscure/configuration/default_configuration.h"
#include <fstream>
#include <iostream>
#include <cmath>


struct triangles_app : obscure::application_2d
{
	static constexpr obscure::version app_version{ 1,0,0 };
	obscure::vulkan::memory_owning_buffer vertex_buffer;

	std::array<obscure::shape::colored_vertex, 3> vertices = 
	{
		obscure::shape::colored_vertex({0.0f, -0.1f}, {1.0f, 1.0f, 1.0f}),
		obscure::shape::colored_vertex({0.1f, 0.1f}, {0.0f, 1.0f, 0.0f}),
		obscure::shape::colored_vertex({-0.1f, 0.1f}, {0.0f, 0.0f, 1.0f})
	};

	triangles_app()
		: obscure::application_2d(obscure::configuration::default_configuration(800, 800, "triangles", app_version, true))
	{
		vertex_buffer = context.create_coherent_vertex_buffer<obscure::shape::colored_vertex>(vertices.size());
	}

	void loop(obscure::vulkan::command_sequence draw_context)
	{
		static obscure::stopwatch frame_timer;
		auto elapsed_time = frame_timer.elapsed_time();
		float cos = std::cos(elapsed_time.count());

		float top = -0.12f * cos - 0.1f;
		vertices[0].vertex.y = top;

		float bottom = -0.1f * cos + 0.1f;
		vertices[1].vertex.y = bottom;
		vertices[2].vertex.y = bottom;


		draw_context
			.begin_rendering()
			.draw_colored_triangle_list(vertices, vertex_buffer)
			.end_rendering();
		draw_context.submit_commands();
		draw_context.display();
	}
};

int main()
{
	triangles_app app;
	app.run();
	return 0;
}