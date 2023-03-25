#include "obscure/application_2d.h"
#include "obscure/configuration/default_configuration.h"
#include <fstream>
#include <iostream>


struct triangles_app : obscure::application_2d
{
	static constexpr obscure::version app_version{ 1,0,0 };
	triangles_app()
		: obscure::application_2d(obscure::configuration::default_configuration(800, 800, "triangles", app_version))
	{}

	void loop(obscure::vulkan::command_sequence draw_context, obscure::stopwatch::seconds elapsed_time)
	{
		draw_context.begin_rendering().draw_static_triangle().end_rendering();
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