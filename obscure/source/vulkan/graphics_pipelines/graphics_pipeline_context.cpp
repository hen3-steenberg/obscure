module obscure.vulkan.graphics_pipeline_context;

const VkPipeline obscure::vulkan::graphics_pipeline_context::pipeline() const&
{
	return pipelines()[pipeline_index()];
}

obscure::internal::vulkan::pipeline_layout const& obscure::vulkan::graphics_pipeline_context::layout() const&
{
	return layouts()[pipeline_index()];
}