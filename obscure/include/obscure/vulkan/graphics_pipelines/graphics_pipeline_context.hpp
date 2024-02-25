module;
#include <type_traits>
#include "glfw_vulkan_include.h"
export module obscure.vulkan.graphics_pipeline_context;
import obscure.internal.vulkan.pipeline_layout;

namespace obscure
{
	namespace vulkan
	{
		export struct graphics_pipeline_context
		{
		protected:
			virtual size_t pipeline_index() const& = 0;
			virtual VkCommandBuffer command_buffer() const& = 0;
			virtual VkExtent2D extent() const& = 0;
			virtual VkPipeline* pipelines() const& = 0;
			const VkPipeline pipeline() const&;
			virtual obscure::internal::vulkan::pipeline_layout const* layouts() const& = 0;
			obscure::internal::vulkan::pipeline_layout const& layout() const&;
			
		};

		export
		template<typename T>
		concept pipeline_context = std::is_base_of_v<graphics_pipeline_context, T> && std::is_default_constructible_v<T>;
	}
}