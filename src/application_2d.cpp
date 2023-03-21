#include "obscure/application_2d.h"
#include "obscure/vulkan/device_picker.h"

VkPhysicalDevice obscure::application_2d::pick_device()
{
	vulkan::device_picker picker(vk_instance, vk_surface);
	return picker.pick_device();
}

obscure::application_2d::application_2d(int width, int height, const char* title, version Version)
	: window(width, height, title),
	vk_instance(title, Version),
	vk_surface(vk_instance, window),
	vk_device(pick_device(), vk_surface),
	swap_chain(vk_device)
#if ENABLE_VALIDATION_LAYERS
	, console_logger(vk_instance),
	csv_logger(vk_instance, "vulkan_log.csv")
#endif
{

}

void obscure::application_2d::run()
{
	stopwatch sw;
	while (!do_exit)
	{
		loop(sw.lap_time());
		window.poll_events();
		if (window.should_close())
		{
			exit();
		}
	}
}

void obscure::application_2d::exit() noexcept
{
	do_exit = true;
}