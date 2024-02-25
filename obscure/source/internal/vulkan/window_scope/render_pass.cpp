module;
#include "glfw_vulkan_include.h"
module obscure.internal.vulkan.render_pass;


obscure::internal::vulkan::render_pass::render_pass() noexcept
	: vk_render_pass(VK_NULL_HANDLE)
{}

obscure::internal::vulkan::render_pass::render_pass(device device, VkFormat format, VkAllocationCallbacks const* allocator)
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

	vkCreateRenderPass(device.get_handle(), &create_info, allocator, &vk_render_pass);
}

void obscure::internal::vulkan::render_pass::free(device _device, VkAllocationCallbacks const* allocator) noexcept
{
	vkDestroyRenderPass(_device.get_handle(), vk_render_pass, allocator);
}

VkRenderPass obscure::internal::vulkan::render_pass::get_handle() const noexcept
{
	return vk_render_pass;
}