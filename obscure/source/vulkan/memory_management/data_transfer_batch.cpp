module;
#include "glfw_vulkan_include.h"
module obscure.vulkan.data_transfer_batch;

obscure::vulkan::data_transfer_batch::data_transfer_batch(obscure::internal::vulkan::device device, obscure::internal::vulkan::command_pool transfer_cmd_pool)
	: transfer_queue(device.get_transfer_queue()),
	transfer_pool(transfer_cmd_pool),
	transfer_completed(device, false),
	transfer_commands(transfer_pool.allocate_primary_command_buffer(device))
{}

obscure::vulkan::data_transfer_batch::data_transfer_batch(data_transfer_batch&& other) noexcept
	: transfer_queue(other.transfer_queue),
	transfer_pool(other.transfer_pool),
	transfer_completed(other.transfer_completed),
	transfer_commands(other.transfer_commands)
{
	other.transfer_commands = VK_NULL_HANDLE;
}

void obscure::vulkan::data_transfer_batch::record_transfer(obscure::internal::vulkan::memory_owning_buffer const& src, obscure::internal::vulkan::memory_owning_buffer const& dst, size_t size)&
{
	VkBufferCopy copy_region{};
	copy_region.srcOffset = 0;
	copy_region.dstOffset = 0;
	copy_region.size = size;

	vkCmdCopyBuffer(transfer_commands, src.get_handle(), dst.get_handle(), 1, &copy_region);
}

void obscure::vulkan::data_transfer_batch::begin_transfers()&
{
	vkEndCommandBuffer(transfer_commands);

	VkSubmitInfo transfer_info{};
	transfer_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	transfer_info.pNext = nullptr;
	transfer_info.waitSemaphoreCount = 0;
	transfer_info.pWaitSemaphores = nullptr;
	transfer_info.pWaitDstStageMask = nullptr;
	transfer_info.commandBufferCount = 1;
	transfer_info.pCommandBuffers = &transfer_commands;
	transfer_info.signalSemaphoreCount = 0;
	transfer_info.pSignalSemaphores = nullptr;

	vkQueueSubmit(transfer_queue, 1, &transfer_info, transfer_completed.get_handle());

}

void obscure::vulkan::data_transfer_batch::wait_for_transfers()&
{
	transfer_completed.wait();
}

obscure::vulkan::data_transfer_batch::~data_transfer_batch()
{
	if (transfer_commands != VK_NULL_HANDLE)
	{
		vkFreeCommandBuffers(transfer_completed.vk_device, transfer_pool.get_handle(), 1, &transfer_commands);

		transfer_completed.free();
	}
	transfer_commands = VK_NULL_HANDLE;
}