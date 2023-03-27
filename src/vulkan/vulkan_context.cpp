#include "obscure/vulkan/vulkan_context.h"

obscure::vulkan::vulkan_context::vulkan_context() noexcept
	: data()
{}

void obscure::vulkan::vulkan_context::poll_events() const noexcept
{
	data->window.poll_events();
}

bool obscure::vulkan::vulkan_context::should_close() const noexcept
{
	return data->window.should_close();
}

obscure::vulkan::command_sequence obscure::vulkan::vulkan_context::get_next_frame_context()
{
	return obscure::vulkan::command_sequence(data.get());
}

bool obscure::vulkan::vulkan_context::is_minimized() const
{
	return data->window.is_minimized();
}