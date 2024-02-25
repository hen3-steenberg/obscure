module;
#include "glfw_vulkan_include.h"
module obscure.internal.vulkan.frame_data;

obscure::internal::vulkan::frame_data::frame_data(VkPhysicalDevice physical_device, surface _surface, VkExtent2D native_resolution)
	: surface(_surface),
	device(physical_device, surface),
	swap_chain(device, surface, native_resolution),
	graphics_queue(device.get_graphics_queue()),
	present_queue(device.get_present_queue()),
	graphics_command_pool(command_pool::create_graphics_pool(device)),
	current_frame(),
	should_resize(false)
{
	graphics_command_pool.allocate_primary_command_buffers(device, graphics_command_buffers);
	for (size_t index = 0; index < obscure::maximum_frames_in_flight(); ++index)
	{
		draw_complete_fences[index] = fence(device, true);
		ready_to_draw_semaphores[index] = semaphore(device);
		ready_to_display_semaphores[index] = semaphore(device);
	}
}

void obscure::internal::vulkan::frame_data::free(instance instance) noexcept
{
	for (size_t index = 0; index < obscure::maximum_frames_in_flight(); ++index)
	{
		draw_complete_fences[index].free();
		ready_to_draw_semaphores[index].free(device);
		ready_to_display_semaphores[index].free(device);
	}
	graphics_command_pool.free(device);
	swap_chain.free(device);
	device.free();
	surface.free(instance);
}

void obscure::internal::vulkan::frame_data::wait_for_device_idle()&
{
	device.wait_for_idle();
}

uint32_t obscure::internal::vulkan::frame_data::get_next_frame()&
{
	return current_frame++;
}

uint32_t obscure::internal::vulkan::frame_data::get_next_buffer_index(uint32_t current_frame)&
{
	return swap_chain.get_next_image_index(device, ready_to_draw_semaphores[current_frame]);
}

VkCommandBuffer obscure::internal::vulkan::frame_data::get_current_command_buffer(uint32_t current_frame) const&
{
	return graphics_command_buffers[current_frame];
}