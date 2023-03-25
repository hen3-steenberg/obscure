#include "obscure/vulkan/pipeline_collection.h"
#include "obscure/vulkan/application_context.h"
#include "obscure/vulkan/graphics_pipeline_configuration.h"
#include <array>


struct static_triangle_configuration : obscure::vulkan::graphics_pipeline_configuration
{
	obscure::vulkan::application_context const* ctx;
	VkAllocationCallbacks const* allocator;

	std::array<VkPipelineShaderStageCreateInfo, 2> shaders;
	static constexpr std::array<VkDynamicState, 2> _dynamic_state = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };


	VkPipelineDynamicStateCreateInfo _dynamic_state_info{};

	VkPipelineVertexInputStateCreateInfo _vertex_input_state{};

	VkPipelineInputAssemblyStateCreateInfo _input_assembly{};

	VkPipelineColorBlendAttachmentState color_blend_attatchment{};
	VkPipelineColorBlendStateCreateInfo _color_blend_state{};

	VkViewport viewport{};
	VkRect2D scissor{};
	VkPipelineViewportStateCreateInfo _viewport_state{};

	VkPipelineMultisampleStateCreateInfo _multisample_state{};

	VkPipelineRasterizationStateCreateInfo _rasterization_state{};

	obscure::vulkan::pipeline_layout _layout;

	obscure::vulkan::pipeline_layout layout()
	{
		if (_layout.get_handle() == VK_NULL_HANDLE)
		{
			VkPipelineLayoutCreateInfo create_info{};
			create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
			create_info.pNext = nullptr;

			create_info.setLayoutCount = 0; // Optional
			create_info.pSetLayouts = nullptr; // Optional
			create_info.pushConstantRangeCount = 0; // Optional
			create_info.pPushConstantRanges = nullptr; // Optional

			_layout = obscure::vulkan::pipeline_layout(ctx->device, create_info, allocator);
		}
		return _layout;
	}

	std::span<VkPipelineShaderStageCreateInfo> shader_stages()
	{
		return shaders;
	}

	VkPipelineVertexInputStateCreateInfo* vertex_input_state()
	{
		return &_vertex_input_state;
	}

	VkPipelineDynamicStateCreateInfo* dynamic_state()
	{
		return &_dynamic_state_info;
	}

	VkPipelineInputAssemblyStateCreateInfo* input_assembly_state()
	{
		return &_input_assembly;
	}

	VkPipelineViewportStateCreateInfo* viewport_state()
	{
		return &_viewport_state;
	}

	VkPipelineMultisampleStateCreateInfo* multisample_state()
	{
		return &_multisample_state;
	}

	VkPipelineRasterizationStateCreateInfo* rasterization_state()
	{
		return &_rasterization_state;
	}

	VkPipelineColorBlendStateCreateInfo* color_blend_state()
	{
		return &_color_blend_state;
	}

	VkPipelineDepthStencilStateCreateInfo* depth_stencil_state()
	{
		return nullptr;
	}

	static_triangle_configuration(obscure::vulkan::application_context const*  context, VkPipelineShaderStageCreateInfo vertex, VkPipelineShaderStageCreateInfo fragment, VkAllocationCallbacks const* _allocator )
		: ctx(context), allocator(_allocator), shaders{vertex, fragment}
	{
		_dynamic_state_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		_dynamic_state_info.pNext = nullptr;
		_dynamic_state_info.flags = 0;
		_dynamic_state_info.dynamicStateCount = _dynamic_state.size();
		_dynamic_state_info.pDynamicStates = _dynamic_state.data();

		_vertex_input_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		_vertex_input_state.pNext = nullptr;
		_vertex_input_state.vertexBindingDescriptionCount = 0;
		_vertex_input_state.pVertexBindingDescriptions = nullptr; // Optional
		_vertex_input_state.vertexAttributeDescriptionCount = 0;
		_vertex_input_state.pVertexAttributeDescriptions = nullptr; // Optional

		_input_assembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		_input_assembly.pNext = nullptr;
		_input_assembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		_input_assembly.primitiveRestartEnable = VK_FALSE;

		
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = (float)ctx->swap_chain.extent.width;
		viewport.height = (float)ctx->swap_chain.extent.height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		
		scissor.offset = { 0, 0 };
		scissor.extent = ctx->swap_chain.extent;

		_viewport_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		_viewport_state.pNext = nullptr;
		_viewport_state.flags = 0;

		_viewport_state.viewportCount = 1;
		_viewport_state.pViewports = &viewport;

		_viewport_state.scissorCount = 1;
		_viewport_state.pScissors = &scissor;

		_multisample_state.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		_multisample_state.pNext = nullptr;
		_multisample_state.flags = 0;

		_multisample_state.sampleShadingEnable = VK_FALSE;
		_multisample_state.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		_multisample_state.minSampleShading = 1.0f; // Optional
		_multisample_state.pSampleMask = nullptr; // Optional
		_multisample_state.alphaToCoverageEnable = VK_FALSE; // Optional
		_multisample_state.alphaToOneEnable = VK_FALSE; // Optional

		_rasterization_state.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		_rasterization_state.pNext = nullptr;
		_rasterization_state.flags = 0;

		_rasterization_state.depthClampEnable = VK_FALSE;
		_rasterization_state.rasterizerDiscardEnable = VK_FALSE;
		_rasterization_state.polygonMode = VK_POLYGON_MODE_FILL;
		_rasterization_state.lineWidth = 1.0f;
		_rasterization_state.cullMode = VK_CULL_MODE_BACK_BIT;
		_rasterization_state.frontFace = VK_FRONT_FACE_CLOCKWISE;
		_rasterization_state.depthBiasEnable = VK_FALSE;
		_rasterization_state.depthBiasConstantFactor = 0.0f; // Optional
		_rasterization_state.depthBiasClamp = 0.0f; // Optional
		_rasterization_state.depthBiasSlopeFactor = 0.0f; // Optional

		
		color_blend_attatchment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		color_blend_attatchment.blendEnable = VK_TRUE;
		color_blend_attatchment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
		color_blend_attatchment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		color_blend_attatchment.colorBlendOp = VK_BLEND_OP_ADD;
		color_blend_attatchment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		color_blend_attatchment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		color_blend_attatchment.alphaBlendOp = VK_BLEND_OP_ADD;

		_color_blend_state.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		_color_blend_state.pNext = nullptr;
		_color_blend_state.flags = 0;

		_color_blend_state.logicOpEnable = VK_FALSE;
		_color_blend_state.logicOp = VK_LOGIC_OP_COPY; // Optional
		_color_blend_state.attachmentCount = 1;
		_color_blend_state.pAttachments = &color_blend_attatchment;
		_color_blend_state.blendConstants[0] = 0.0f; // Optional
		_color_blend_state.blendConstants[1] = 0.0f; // Optional
		_color_blend_state.blendConstants[2] = 0.0f; // Optional
		_color_blend_state.blendConstants[3] = 0.0f; // Optional
	}
};



obscure::vulkan::pipeline_collection::pipeline_collection()
	: static_triangle_vertex(), static_triangle_fragment(),
	static_triangle_layout(),
	static_triangle_pipeline(VK_NULL_HANDLE)
{}

obscure::vulkan::pipeline_collection::pipeline_collection(application_context const* context, VkAllocationCallbacks const* allocator)
	: static_triangle_vertex(context->device, "shaders/static.vert.spv", allocator), 
	static_triangle_fragment(context->device, "shaders/static.frag.spv", allocator)
{
	static_triangle_configuration static_triangle_config(context, static_triangle_vertex.configure_stage(VK_SHADER_STAGE_VERTEX_BIT), static_triangle_fragment.configure_stage(VK_SHADER_STAGE_FRAGMENT_BIT),  allocator);

	static_triangle_layout = static_triangle_config.layout();

	std::array<VkGraphicsPipelineCreateInfo, 1> pipeline_create_infos = { static_triangle_config.parse_configuration(context->swap_chain.render_pass) };
	
	vkCreateGraphicsPipelines(context->device.get_handle(), VK_NULL_HANDLE, static_cast<uint32_t>(pipeline_create_infos.size()), pipeline_create_infos.data(), allocator, pipelines);


}

void obscure::vulkan::pipeline_collection::free(device device, VkAllocationCallbacks const* allocator) noexcept
{
	vkDestroyPipeline(device.get_handle(), static_triangle_pipeline, allocator);
	static_triangle_layout.free(device, allocator);
	static_triangle_fragment.free(device, allocator);
	static_triangle_vertex.free(device, allocator);
}