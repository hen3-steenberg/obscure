#include "obscure/vulkan/swap_chain.h"
#include "obscure/vulkan/templates.hpp"

obscure::vulkan::render_pass create_render_pass(obscure::vulkan::device dev, VkFormat format, VkAllocationCallbacks const* allocator)
{
	VkAttachmentDescription colorAttachment{};
	colorAttachment.format = format;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;

	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference colorAttachmentRef{};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass{};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;

	VkRenderPassCreateInfo create_info{};

	create_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	create_info.pNext = nullptr;
	create_info.attachmentCount = 1;
	create_info.pAttachments = &colorAttachment;
	create_info.subpassCount = 1;
	create_info.pSubpasses = &subpass;

	return obscure::vulkan::render_pass(dev, create_info, allocator);
}
 
obscure::vulkan::swap_chain::swap_chain() noexcept
	: vk_swap_chain(VK_NULL_HANDLE), swap_chain_images(), swap_chain_views(), image_format(VK_FORMAT_B8G8R8A8_SRGB), extent(), render_pass()
{}


obscure::vulkan::swap_chain::swap_chain(device device, VkSwapchainCreateInfoKHR create_info, VkAllocationCallbacks const* allocator)
	: image_format(create_info.imageFormat), extent(create_info.imageExtent), render_pass(create_render_pass(device, create_info.imageFormat, allocator))
{
	VkResult Err = vkCreateSwapchainKHR(device.get_handle(), &create_info, nullptr, &vk_swap_chain);
	if (Err != VK_SUCCESS) throw Err;

	swap_chain_images = vulkan_load(device.get_handle(), vk_swap_chain, vkGetSwapchainImagesKHR);

	swap_chain_views.reserve(swap_chain_images.size());

	for (auto image : swap_chain_images)
	{
		swap_chain_views.emplace_back(device, image, image_format);
	}

	frame_buffers.reserve(swap_chain_images.size());

	for (auto view : swap_chain_views)
	{
		frame_buffers.emplace_back(device, view, render_pass, extent, allocator);
	}
}

VkSwapchainKHR obscure::vulkan::swap_chain::get_handle() const noexcept
{
	return vk_swap_chain;
}

void obscure::vulkan::swap_chain::free(device device, VkAllocationCallbacks const* allocator) noexcept
{

	for (auto buffer : frame_buffers)
	{
		buffer.free(device, allocator);
	}

	for (auto image_view : swap_chain_views)
	{
		image_view.free(device, allocator);
	}

	render_pass.free(device, allocator);

	vkDestroySwapchainKHR(device.get_handle(), vk_swap_chain, allocator);
}