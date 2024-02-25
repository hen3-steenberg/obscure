module;
module obscure.internal.vulkan.frame_context;

obscure::internal::vulkan::frame_context::frame_context(frame_data& _data)
	: data(_data),
	current_frame(data.get_next_frame()),
	current_buffer_index(data.get_next_buffer_index(current_frame))
{
	data.draw_complete_fences[current_frame].wait();

	if (data.should_resize)
	{
		data.swap_chain = obscure::internal::vulkan::swap_chain(data.device, data.surface, )
	}
}

obscure::internal::vulkan::frame_context::frame_context(frame_context&& other) noexcept
	: data(other.data),
	_command_buffer(other._command_buffer),
	_extent(other._extent),
	current_buffer_index(other.current_buffer_index),
	current_frame(other.current_frame)

{
	other._command_buffer = VK_NULL_HANDLE;
}

VkCommandBuffer obscure::internal::vulkan::frame_context::command_buffer() const&
{
	return data.get_current_command_buffer(current_frame);
}

VkExtent2D obscure::internal::vulkan::frame_context::extent() const&
{
	return data.swap_chain.extent;
}