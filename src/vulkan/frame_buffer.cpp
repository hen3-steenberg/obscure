#include "obscure/vulkan/frame_buffer.h"

obscure::vulkan::frame_buffer::frame_buffer() noexcept
	: vk_frame_buffer(VK_NULL_HANDLE)
{}

obscure::vulkan::frame_buffer::frame_buffer(device device, image_view view, render_pass pass, VkExtent2D extent, VkAllocationCallbacks const* allocator)
{
    VkFramebufferCreateInfo framebufferInfo{};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.pNext = nullptr;
    framebufferInfo.flags = 0;

    framebufferInfo.renderPass = pass.get_handle();
    framebufferInfo.attachmentCount = 1;
    framebufferInfo.pAttachments = view.get_handle_address();
    framebufferInfo.width = extent.width;
    framebufferInfo.height = extent.height;
    framebufferInfo.layers = 1;

    vkCreateFramebuffer(device.get_handle(), &framebufferInfo, allocator, &vk_frame_buffer);
}

VkFramebuffer obscure::vulkan::frame_buffer::get_handle() const noexcept
{
    return vk_frame_buffer;
}

void obscure::vulkan::frame_buffer::free(device device, VkAllocationCallbacks const* allocator) noexcept
{
    vkDestroyFramebuffer(device.get_handle(), vk_frame_buffer, allocator);
}