#include "obscure/application_2d.h"
#include "obscure/configuration/default_configuration.h"
#include <fstream>
#include <iostream>
#include <cmath>


struct squares_app : obscure::application_2d
{
	static constexpr obscure::version app_version{ 1,0,0 };
	obscure::vulkan::vertex_array<obscure::shape::colored_vertex> vertices;
	obscure::vulkan::index_array<VK_INDEX_TYPE_UINT16> indices;

	squares_app()
		: obscure::application_2d(obscure::configuration::default_configuration(800, 800, "triangles", app_version, true))
	{
		vertices = context.create_vertex_array<obscure::shape::colored_vertex>({
				obscure::shape::colored_vertex({ -0.1f, -0.1f }, { 1.0f, 1.0f, 1.0f }),
				obscure::shape::colored_vertex({ 0.1f, -0.1f }, { 1.0f, 1.0f, 1.0f }),
				obscure::shape::colored_vertex({ 0.1f, 0.1f }, { 0.0f, 1.0f, 0.0f }),
				obscure::shape::colored_vertex({ -0.1f, 0.1f }, { 0.0f, 0.0f, 1.0f })
			});

		indices = context.create_index_array<VK_INDEX_TYPE_UINT16>({0, 1, 2, 2, 3, 0});
	}

	void loop(obscure::vulkan::command_sequence draw_context)
	{
		static obscure::stopwatch frame_timer;
		auto elapsed_time = frame_timer.elapsed_time();
		float cos = std::cos(4 * elapsed_time.count());

		float top = -0.12f * cos - 0.1f;
		vertices[0].vertex.y = top;
		vertices[1].vertex.y = top;

		float bottom = -0.1f * cos + 0.1f;
		vertices[2].vertex.y = bottom;
		vertices[3].vertex.y = bottom;

		auto transfer_ctx = draw_context.begin_transfers();
		vertices.transfer_to_device(transfer_ctx);
		indices.transfer_to_device(transfer_ctx);
		transfer_ctx.finalize_transfers();

		draw_context
			.begin_rendering()
			.draw_colored_triangle_list<VK_INDEX_TYPE_UINT16>(vertices, indices)
			.end_rendering();

		draw_context.submit_commands();
		draw_context.display();
	}
};

int main()
{
	squares_app app;
	app.run();
	return 0;
}