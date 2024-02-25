module;
#include "glfw_vulkan_include.h"

export module obscure.internal.vulkan.render_context:pipeline;
import obscure.vulkan.graphics_pipeline_context;
import obscure.internal.vulkan.pipeline_layout;

namespace obscure
{
	namespace internal
	{
		namespace vulkan
		{
			export template<size_t Index, obscure::vulkan::pipeline_context Tpipeline>
				struct pipeline_implementation : public Tpipeline
			{
			private:
				size_t pipeline_index() const& override
				{
					return Index;
				}
			protected:
				virtual VkCommandBuffer command_buffer() const& override = 0;
				virtual VkExtent2D extent() const& override = 0;
				virtual VkPipeline const* pipelines() const& override = 0;
				virtual obscure::internal::vulkan::pipeline_layout const* layouts() const& override = 0;
			};

			

			export template<size_t Index, obscure::vulkan::pipeline_context Tpipeline, obscure::vulkan::pipeline_context ... Tpipelines>
				struct pipeline_group_impl : public pipeline_implementation<Index, Tpipeline>,public pipeline_group_impl<Index + 1, Tpipelines...>
			{
			};

			export template<obscure::vulkan::pipeline_context ... Tpipelines>
				using pipeline_group = pipeline_group_impl<0, Tpipelines...>;
			
		}
	}
}