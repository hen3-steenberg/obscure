#include "obscure/vulkan/application_context.h"
#include "obscure/configuration/configuration_provider.h"

obscure::vulkan::application_context::application_context(obscure::configuration::configuration_provider* configuration)
{
	configuration::application_configuration* app_configuration = configuration->load_application_configuration();

	

	window = vulkan::glfw_window(app_configuration->window_width(), app_configuration->window_height(), app_configuration->application_title());

	instance = vulkan::instance(app_configuration->application_title(), app_configuration->application_version());

	configuration::vulkan_configuration* vulkan_config = configuration->load_vulkan_configuration(this);

	loggers = vulkan_config->vulkan_logging();

	surface = vulkan::surface(instance, window);

	physical_device = vulkan_config->pick_device();

	device = vulkan::device(physical_device, surface);

	graphics_queue = device.get_graphics_queue();

	present_queue = device.get_present_queue();

	draw_complete = vulkan::fence(device, true);

	ready_to_draw = vulkan::semaphore(device);

	ready_to_display = vulkan::semaphore(device);

	swap_chain = vulkan::swap_chain(device, vulkan_config->configure_swap_chain());

	pipelines = vulkan::pipeline_collection{ this };

	graphics_command_pool = vulkan::command_pool(device, device.graphics_queue_index);

	graphics_command_buffer = graphics_command_pool.allocate_primary_command_buffer(device);

}

obscure::vulkan::application_context::~application_context()
{
	device.wait_for_idle();

	graphics_command_pool.free(device);

	pipelines.free(device);

	swap_chain.free(device);

	ready_to_display.free(device);

	ready_to_draw.free(device);

	draw_complete.free();

	device.free();

	surface.free(instance);

	loggers.free(instance);

	instance.free();

	window.free();
}