#include "obscure/vulkan/pipeline_collection.h"
#include "obscure/vulkan/application_context.h"
#include "obscure/vulkan/pipeline_configuration/static_triangle_configuration.h"
#include "obscure/vulkan//pipeline_configuration/colored_triangle_list_configuration.h"
#include <array>
#include <filesystem>
#include <unordered_map>


void load_shaders(obscure::vulkan::device device, const VkAllocationCallbacks* allocator, std::span<std::filesystem::path> shader_paths, std::unordered_map<std::filesystem::path, obscure::vulkan::shader_module> & loaded_shaders)
{
	for (auto const& path : shader_paths)
	{
		if (!loaded_shaders.contains(path))
		{
			loaded_shaders[path] = obscure::vulkan::shader_module(device, path, allocator);
		}
	}
}


obscure::vulkan::pipeline_collection::pipeline_collection()
	: shaders(),
	static_triangle_layout(),
	colored_triangle_list_layout(),
	static_triangle_pipeline(VK_NULL_HANDLE),
	colored_triangle_list_pipeline(VK_NULL_HANDLE)
{}

obscure::vulkan::pipeline_collection::pipeline_collection(application_context const* context, VkAllocationCallbacks const* allocator)
	: shaders()
{
	

	static_triangle_configuration static_triangle_config(context->device, context->swap_chain, allocator);
	colored_triangle_list_configuration colored_triangle_list_config(context->device, context->swap_chain, allocator);

	static_triangle_layout = static_triangle_config.layout();
	colored_triangle_list_layout = colored_triangle_list_config.layout();

	std::unordered_map<std::filesystem::path, obscure::vulkan::shader_module> loaded_shaders;

	load_shaders(context->device, allocator, static_triangle_config.shader_stage_paths(), loaded_shaders);
	load_shaders(context->device, allocator, colored_triangle_list_config.shader_stage_paths(), loaded_shaders);

	shaders.reserve(loaded_shaders.size());

	for (auto const & shader : loaded_shaders)
	{
		shaders.push_back(shader.second);
	}

	std::array<VkGraphicsPipelineCreateInfo, 2> pipeline_create_infos = 
	{ 
		static_triangle_config.parse_configuration(context->swap_chain.render_pass, loaded_shaders), 
		colored_triangle_list_config.parse_configuration(context->swap_chain.render_pass, loaded_shaders)
	};
	
	vkCreateGraphicsPipelines(context->device.get_handle(), VK_NULL_HANDLE, static_cast<uint32_t>(pipeline_create_infos.size()), pipeline_create_infos.data(), allocator, pipelines);
}

void obscure::vulkan::pipeline_collection::free(device device, VkAllocationCallbacks const* allocator) noexcept
{
	vkDestroyPipeline(device.get_handle(), static_triangle_pipeline, allocator);
	vkDestroyPipeline(device.get_handle(), colored_triangle_list_pipeline, allocator);
	static_triangle_layout.free(device, allocator);
	colored_triangle_list_layout.free(device, allocator);
	for (auto & shader : shaders)
	{
		shader.free(device, allocator);
	}
}