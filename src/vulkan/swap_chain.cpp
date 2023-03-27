#include "obscure/vulkan/swap_chain.h"
#include "obscure/vulkan/swap_chain_configuration.h"
#include "obscure/vulkan/application_context.h"
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

	VkSubpassDependency dependency{};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;

	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;

	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

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

	create_info.dependencyCount = 1;
	create_info.pDependencies = &dependency;

	return obscure::vulkan::render_pass(dev, create_info, allocator);
}
 
obscure::vulkan::swap_chain::swap_chain() noexcept
	: vk_swap_chain(VK_NULL_HANDLE), swap_chain_images(), swap_chain_views(), image_format(VK_FORMAT_B8G8R8A8_SRGB), extent(), render_pass()
{}


obscure::vulkan::swap_chain::swap_chain(application_context const* context, VkAllocationCallbacks const* allocator)
{
	swap_chain_configuration config{ context };

	VkSwapchainCreateInfoKHR create_info = config.parse_configuration();

	image_format = create_info.imageFormat;
	extent = create_info.imageExtent;

	render_pass = create_render_pass(context->device, image_format, allocator);

	VkResult Err = vkCreateSwapchainKHR(context->device.get_handle(), &create_info, nullptr, &vk_swap_chain);
	if (Err != VK_SUCCESS) throw Err;

	swap_chain_images = vulkan_load(context->device.get_handle(), vk_swap_chain, vkGetSwapchainImagesKHR);

	swap_chain_views.reserve(swap_chain_images.size());

	for (auto image : swap_chain_images)
	{
		swap_chain_views.emplace_back(context->device, image, image_format);
	}

	frame_buffers.reserve(swap_chain_images.size());

	for (auto view : swap_chain_views)
	{
		frame_buffers.emplace_back(context->device, view, render_pass, extent, allocator);
	}
}

VkSwapchainKHR obscure::vulkan::swap_chain::get_handle() const noexcept
{
	return vk_swap_chain;
}

VkSwapchainKHR const* obscure::vulkan::swap_chain::get_handle_address() const noexcept
{
	return &vk_swap_chain;
}

void obscure::vulkan::swap_chain::free(device device, VkAllocationCallbacks const* allocator) noexcept
{
	free_collection<frame_buffer>(frame_buffers, device, allocator);

	free_collection<image_view>(swap_chain_views, device, allocator);

	render_pass.free(device, allocator);

	vkDestroySwapchainKHR(device.get_handle(), vk_swap_chain, allocator);
}


void obscure::vulkan::swap_chain::recreate(application_context * context, VkAllocationCallbacks const* allocator)
{
	context->device.wait_for_idle();
	free(context->device, allocator);
	frame_buffers.clear();
	swap_chain_views.clear();
	swap_chain_images.clear();

	new (this) swap_chain{ context, allocator };//placement new to construct the new swap_chain in place
	context->frame_buffer_resized = false;
}

uint32_t obscure::vulkan::swap_chain::get_next_image_index(device device, semaphore image_ready_signal)
{
	uint32_t imageIndex;
	VkResult Err = vkAcquireNextImageKHR(device.get_handle(), vk_swap_chain, UINT64_MAX, image_ready_signal.get_handle(), VK_NULL_HANDLE, &imageIndex);
	if (Err != VK_SUCCESS)
	{
		throw Err;
	}
	return imageIndex;
}