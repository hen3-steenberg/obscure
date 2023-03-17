#include "obscure/application_2d.h"


struct triangles_app : obscure::application_2d
{
	static constexpr obscure::version app_version{ 1,0,0 };
	triangles_app()
		: obscure::application_2d(800, 800, "triangles", app_version)
	{}

	void loop(obscure::stopwatch::seconds elapsed_time)
	{

	}
};

int main()
{
	triangles_app app;
	app.run();
	return 0;
}