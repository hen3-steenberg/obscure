#include "obscure/application_2d.h"

void obscure::application_2d::run()
{
	stopwatch sw;
	while (!do_exit)
	{
		loop(sw.lap_time());
		context.poll_events();
		if (context.should_close())
		{
			exit();
		}
	}
}

void obscure::application_2d::exit() noexcept
{
	do_exit = true;
}