module;
#include "glfw_vulkan_include.h"
export module obscure.internal.vulkan.render_context;
export import :pipeline;
import obscure.vulkan.graphics_pipeline_context;
import obscure.internal.vulkan.pipeline_collection;
import obscure.internal.vulkan.pipeline_layout;

namespace obscure
{
	namespace internal
	{
		namespace vulkan
		{
			template<obscure::vulkan::pipeline_context ... Tpipelines>
			struct render_context final : public pipeline_group<Tpipelines...>
			{
			private:

				VkPipeline const* pipelines() const& override
				{
					return pipeline_data.pipelines();
				}

				pipeline_layout const* layouts() const& override
				{
					return pipeline_data.pipeline_layouts();
				}

				VkCommandBuffer command_buffer() const& override
				{
					return _command_buffer;
				}

				VkExtent2D extent() const& override
				{
					return _extent;
				}
			public:


			};
		}
	}
}