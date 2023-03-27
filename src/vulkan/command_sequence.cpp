#include "obscure/vulkan/command_sequence.h"

obscure::vulkan::command_sequence::command_sequence(application_context* ctx)
	: context(ctx)
{
	context->draw_complete_fences[context->current_frame].wait();
	context->draw_complete_fences[context->current_frame].reset();

	current_buffer_index = context->swap_chain.get_next_image_index(context->device, context->ready_to_draw_semaphores[context->current_frame]);


	vkResetCommandBuffer(context->graphics_command_buffers[context->current_frame], 0);

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = 0; // Optional
	beginInfo.pInheritanceInfo = nullptr; // Optional

	vkBeginCommandBuffer(context->graphics_command_buffers[context->current_frame], &beginInfo);
}

obscure::vulkan::render_context obscure::vulkan::command_sequence::begin_rendering()
{
	return obscure::vulkan::render_context(context->graphics_command_buffers[context->current_frame], context->swap_chain.frame_buffers[current_buffer_index], context);
}
void obscure::vulkan::command_sequence::submit_commands()
{
	vkEndCommandBuffer(context->graphics_command_buffers[context->current_frame]);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = context->ready_to_draw_semaphores[context->current_frame].get_handle_address();
	submitInfo.pWaitDstStageMask = waitStages;

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &context->graphics_command_buffers[context->current_frame];

	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = context->ready_to_display_semaphores[context->current_frame].get_handle_address();

	vkQueueSubmit(context->graphics_queue, 1, &submitInfo, context->draw_complete_fences[context->current_frame].get_handle());

	
}

void obscure::vulkan::command_sequence::display()
{
	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.pNext = nullptr;

	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = context->ready_to_display_semaphores[context->current_frame].get_handle_address();

	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = context->swap_chain.get_handle_address();
	presentInfo.pImageIndices = &current_buffer_index;
	presentInfo.pResults = nullptr; // Optional
	vkQueuePresentKHR(context->present_queue, &presentInfo);

	context->current_frame = ++context->current_frame % maximum_frames_in_flight();
}