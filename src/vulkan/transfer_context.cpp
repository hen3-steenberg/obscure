#include "obscure/vulkan/transfer_context.h"

obscure::vulkan::transfer_context::transfer_context(VkCommandBuffer _transfer_buffer, VkQueue _transfer_queue)
	: transfer_buffer(_transfer_buffer), transfer_queue(_transfer_queue)
{
	vkResetCommandBuffer(transfer_buffer, 0);

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = 0; // Optional
	beginInfo.pInheritanceInfo = nullptr; // Optional

	vkBeginCommandBuffer(transfer_buffer, &beginInfo);
}

void transfer_impl(VkCommandBuffer transfer_buffer, obscure::vulkan::memory_owning_buffer* dest, obscure::vulkan::memory_owning_buffer* src)
{
	uint32_t size = (dest->memory.memory_size < src->memory.memory_size) ? dest->memory.memory_size : src->memory.memory_size;
	VkBufferCopy copyRegion{};
	copyRegion.srcOffset = 0; // Optional
	copyRegion.dstOffset = 0; // Optional
	copyRegion.size = size;
	vkCmdCopyBuffer(transfer_buffer, src->get_handle(), dest->get_handle(), 1, &copyRegion);
}

obscure::vulkan::transfer_context& obscure::vulkan::transfer_context::transfer(memory_owning_vertex_buffer& destination, memory_owning_staging_buffer& src)
{
	transfer_impl(transfer_buffer, &destination, &src);
	return *this;
}

obscure::vulkan::transfer_context& obscure::vulkan::transfer_context::transfer(memory_owning_vertex_buffer& destination, memory_owning_mapped_staging_buffer& src)
{
	transfer_impl(transfer_buffer, &destination, &src);
	return *this;
}


void obscure::vulkan::transfer_context::finalize_transfers()
{
	vkEndCommandBuffer(transfer_buffer);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.pNext = nullptr;
	submitInfo.signalSemaphoreCount = 0;
	submitInfo.pSignalSemaphores = nullptr;
	submitInfo.waitSemaphoreCount = 0;
	submitInfo.pWaitSemaphores = 0;

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &transfer_buffer;

	vkQueueSubmit(transfer_queue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(transfer_queue);

}