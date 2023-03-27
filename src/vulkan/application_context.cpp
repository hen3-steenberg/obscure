#include "obscure/vulkan/application_context.h"
#include "obscure/configuration/configuration_provider.h"
#include "obscure/vulkan/templates.hpp"

obscure::vulkan::application_context::application_context(obscure::configuration::configuration_provider* configuration)
	: current_frame(0)
{
	configuration::application_configuration* app_configuration = configuration->load_application_configuration();

	

	window = vulkan::glfw_window(app_configuration, this);

	instance = vulkan::instance(app_configuration->application_title(), app_configuration->application_version());

	configuration::vulkan_configuration* vulkan_config = configuration->load_vulkan_configuration();

	loggers = vulkan_config->vulkan_logging(instance);

	surface = vulkan::surface(instance, window);

	physical_device = vulkan_config->pick_device(instance, surface);

	device = vulkan::device(physical_device, surface);

	graphics_queue = device.get_graphics_queue();

	present_queue = device.get_present_queue();

	draw_complete_fences = initialize_array<vulkan::fence, maximum_frames_in_flight()>(device, true);

	ready_to_draw_semaphores = initialize_array<vulkan::semaphore, maximum_frames_in_flight()>(device);

	ready_to_display_semaphores = initialize_array<vulkan::semaphore, maximum_frames_in_flight()>(device);

	swap_chain = vulkan::swap_chain(this);

	pipelines = vulkan::pipeline_collection{ this };

	graphics_command_pool = vulkan::command_pool(device, device.graphics_queue_index);

	graphics_command_pool.allocate_primary_command_buffers(device, graphics_command_buffers);

}

obscure::vulkan::application_context::~application_context()
{
	device.wait_for_idle();

	graphics_command_pool.free(device);

	pipelines.free(device);

	swap_chain.free(device);


	free_collection<semaphore>(ready_to_display_semaphores, device);

	free_collection<semaphore>(ready_to_draw_semaphores, device);

	free_collection<fence>(draw_complete_fences);

	device.free();

	surface.free(instance);

	loggers.free(instance);

	instance.free();

	window.free();
}