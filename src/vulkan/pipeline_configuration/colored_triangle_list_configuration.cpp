#include "obscure/vulkan/pipeline_configuration/colored_triangle_list_configuration.h"
#include "obscure/vulkan/pipeline_configuration/descriptions.hpp"

static constexpr std::array<VkDynamicState, 2> _dynamic_state = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
static std::array<std::filesystem::path, 2> shader_paths = { std::filesystem::absolute("shaders/colored_vertex.vert.spv") , std::filesystem::absolute("shaders/color.frag.spv") };

obscure::vulkan::colored_triangle_list_configuration::colored_triangle_list_configuration(obscure::vulkan::device device, obscure::vulkan::swap_chain const& swap_chain, VkAllocationCallbacks const* allocator)
{
	_dynamic_state_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	_dynamic_state_info.pNext = nullptr;
	_dynamic_state_info.flags = 0;
	_dynamic_state_info.dynamicStateCount = static_cast<uint32_t>(_dynamic_state.size());
	_dynamic_state_info.pDynamicStates = _dynamic_state.data();

	input_bindings = get_input_bining_desciptions<obscure::shape::colored_vertex>();
	input_attributes = get_input_attribute_descriptions<obscure::shape::colored_vertex>();

	_vertex_input_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	_vertex_input_state.flags = 0;
	_vertex_input_state.pNext = nullptr;
	_vertex_input_state.vertexBindingDescriptionCount = static_cast<uint32_t>(input_bindings.size());
	_vertex_input_state.pVertexBindingDescriptions = input_bindings.data();
	_vertex_input_state.vertexAttributeDescriptionCount = static_cast<uint32_t>(input_attributes.size());
	_vertex_input_state.pVertexAttributeDescriptions = input_attributes.data();

	_input_assembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	_input_assembly.pNext = nullptr;
	_input_assembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	_input_assembly.primitiveRestartEnable = VK_FALSE;


	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(swap_chain.extent.width);
	viewport.height = static_cast<float>(swap_chain.extent.height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;


	scissor.offset = { 0, 0 };
	scissor.extent = swap_chain.extent;

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

	VkPipelineLayoutCreateInfo create_info{};
	create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	create_info.pNext = nullptr;

	create_info.setLayoutCount = 0; // Optional
	create_info.pSetLayouts = nullptr; // Optional
	create_info.pushConstantRangeCount = 0; // Optional
	create_info.pPushConstantRanges = nullptr; // Optional

	_layout = obscure::vulkan::pipeline_layout(device, create_info, allocator);
}

obscure::vulkan::pipeline_layout obscure::vulkan::colored_triangle_list_configuration::layout()
{
	return _layout;
}

std::span<std::filesystem::path> obscure::vulkan::colored_triangle_list_configuration::shader_stage_paths()
{
	return shader_paths;
}

std::span<VkPipelineShaderStageCreateInfo> obscure::vulkan::colored_triangle_list_configuration::shader_stages(std::unordered_map<std::filesystem::path, obscure::vulkan::shader_module> const& loaded_shaders)
{
	shaders[0] = loaded_shaders.at(shader_paths[0]).configure_stage(VK_SHADER_STAGE_VERTEX_BIT);
	shaders[1] = loaded_shaders.at(shader_paths[1]).configure_stage(VK_SHADER_STAGE_FRAGMENT_BIT);

	return shaders;
}

VkPipelineVertexInputStateCreateInfo* obscure::vulkan::colored_triangle_list_configuration::vertex_input_state()
{
	return &_vertex_input_state;
}

VkPipelineDynamicStateCreateInfo* obscure::vulkan::colored_triangle_list_configuration::dynamic_state()
{
	return &_dynamic_state_info;
}

VkPipelineInputAssemblyStateCreateInfo* obscure::vulkan::colored_triangle_list_configuration::input_assembly_state()
{
	return &_input_assembly;
}

VkPipelineViewportStateCreateInfo* obscure::vulkan::colored_triangle_list_configuration::viewport_state()
{
	return &_viewport_state;
}

VkPipelineMultisampleStateCreateInfo* obscure::vulkan::colored_triangle_list_configuration::multisample_state()
{
	return &_multisample_state;
}

VkPipelineRasterizationStateCreateInfo* obscure::vulkan::colored_triangle_list_configuration::rasterization_state()
{
	return &_rasterization_state;
}

VkPipelineColorBlendStateCreateInfo* obscure::vulkan::colored_triangle_list_configuration::color_blend_state()
{
	return &_color_blend_state;
}

VkPipelineDepthStencilStateCreateInfo* obscure::vulkan::colored_triangle_list_configuration::depth_stencil_state()
{
	return nullptr;
}