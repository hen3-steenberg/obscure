#include "obscure/vulkan/render_context.h"

obscure::vulkan::render_context::render_context(VkCommandBuffer _command_buffer, frame_buffer buffer, application_context const* context)
	: command_buffer(_command_buffer), pipelines(context->pipelines), extent(context->swap_chain.extent)
{
	VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.pNext = nullptr;

	renderPassInfo.renderPass = context->swap_chain.render_pass.get_handle();
	renderPassInfo.framebuffer = buffer.get_handle();

	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = context->swap_chain.extent;

	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearColor;

	vkCmdBeginRenderPass(command_buffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
}




obscure::vulkan::render_context& obscure::vulkan::render_context::draw_static_triangle()
{
	vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelines.static_triangle_pipeline);

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(extent.width);
	viewport.height = static_cast<float>(extent.height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(command_buffer, 0, 1, &viewport);

	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = extent;
	vkCmdSetScissor(command_buffer, 0, 1, &scissor);

	vkCmdDraw(command_buffer, 3, 1, 0, 0);

	return *this;
}

obscure::vulkan::render_context& obscure::vulkan::render_context::draw_colored_triangle_list(vertex_array<obscure::shape::colored_vertex>& data)
{
	return draw_colored_triangle_list(data.device_buffer, data.size());
}

obscure::vulkan::render_context& obscure::vulkan::render_context::draw_colored_triangle_list(memory_owning_device_buffer& vertex_buffer, memory_owning_device_buffer& index_buffer, VkIndexType type, size_t size)
{
	vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelines.colored_triangle_list_pipeline);

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(extent.width);
	viewport.height = static_cast<float>(extent.height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(command_buffer, 0, 1, &viewport);

	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = extent;
	vkCmdSetScissor(command_buffer, 0, 1, &scissor);

	VkBuffer vertexBuffers[] = { vertex_buffer.get_handle() };
	VkDeviceSize offsets[] = { 0 };

	vkCmdBindVertexBuffers(command_buffer, 0, 1, vertexBuffers, offsets);

	vkCmdBindIndexBuffer(command_buffer, index_buffer.get_handle(), 0, type);

	vkCmdDrawIndexed(command_buffer, static_cast<uint32_t>(size), 1, 0, 0, 0);

	return *this;
}

obscure::vulkan::render_context& obscure::vulkan::render_context::draw_colored_triangle_list(memory_owning_device_buffer& vertex_buffer, size_t size)
{
	vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelines.colored_triangle_list_pipeline);

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(extent.width);
	viewport.height = static_cast<float>(extent.height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(command_buffer, 0, 1, &viewport);

	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = extent;
	vkCmdSetScissor(command_buffer, 0, 1, &scissor);

	VkBuffer vertexBuffers[] = { vertex_buffer.get_handle() };
	VkDeviceSize offsets[] = { 0 };

	vkCmdBindVertexBuffers(command_buffer, 0, 1, vertexBuffers, offsets);

	vkCmdDraw(command_buffer, static_cast<uint32_t>(size), 1, 0, 0);

	return *this;
}
void obscure::vulkan::render_context::end_rendering()
{
	vkCmdEndRenderPass(command_buffer);
}