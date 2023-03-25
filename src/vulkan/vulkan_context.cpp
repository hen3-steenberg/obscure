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