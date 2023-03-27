#include "obscure/application_2d.h"

void obscure::application_2d::minimized_loop()
{
	glfwWaitEvents();
}

void obscure::application_2d::run()
{
	while (!do_exit)
	{
		if (!context.is_minimized())
		{
			try
			{
				loop(context.get_next_frame_context());
			}
			catch (VkResult Err)
			{
				if (Err != VK_ERROR_OUT_OF_DATE_KHR && Err != VK_SUBOPTIMAL_KHR) throw Err;
			}
		}
		else
		{
			minimized_loop();
		}
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