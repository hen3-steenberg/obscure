module;
#include "glfw_vulkan_include.h"
export module obscure.internal.vulkan.pipeline_references;
import obscure.internal.vulkan.pipeline_layout;

namespace obscure
{
	namespace internal
	{
		namespace vulkan
		{
			export
				struct pipeline_references
			{
				virtual pipeline_layout const* pipeline_layouts() const& = 0;
				virtual VkPipeline const* pipelines() const& = 0;
			};
		}
	}
}