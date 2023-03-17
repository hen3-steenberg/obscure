#ifndef OBSCURE_APPLICATION_2D_DEFINITION
#define OBSCURE_APPLICATION_2D_DEFINITION 1

#include <atomic>
#include <span>
#include "vulkan/device.h"
#include "properties.hpp"
#include "vulkan/logger.h"

namespace obscure
{
	struct application_2d //: canvas
	{
	protected:
		std::atomic_bool do_exit = false;
		vulkan::window_reference window;
		vulkan::instance vk_instance;
		vulkan::surface vk_surface;
		vulkan::device vk_device;
#if ENABLE_VALIDATION_LAYERS
		vulkan::verbose_console_logger console_logger;
		vulkan::verbose_csv_logger csv_logger;
#endif


	public:

		virtual VkPhysicalDevice pick_device();

		application_2d(int width, int height, const char* title, version Version);

		virtual void loop(stopwatch::seconds elapsed_time) = 0;
		void run();

		void exit() noexcept;
	};
}
#endif