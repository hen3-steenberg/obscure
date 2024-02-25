module;
#include <array>
#include <vector>
#include "glfw_vulkan_include.h"
export module obscure.internal.vulkan.pipeline_collection;
import obscure.internal.vulkan.pipeline_layout;
import obscure.internal.vulkan.shader_module;
import obscure.internal.vulkan.device;

namespace obscure
{
	namespace internal
	{
		namespace vulkan
		{
			export
			template<size_t N>
			struct pipeline_collection
			{
				std::vector<shader_module> shader_modules;
				std::array<pipeline_layout, N> layout_data;
				std::array<VkPipeline, N> pipeline_data;

				pipeline_layout const* pipeline_layouts() const& override
				{
					return layout_data.data();
				}

				VkPipeline const* pipelines() const& override
				{
					return pipeline_data.data();
				}

				void free(device device, VkAllocationCallbacks const* allocator = nullptr)
				{
					for (VkPipeline pipeline : pipeline_data)
					{
						vkDestroyPipeline(device.get_handle(), pipeline, allocator);
					}

					for (pipeline_layout layout : layout_data)
					{
						layout.free(device, allocator);
					}

					for (shader_module shader : shader_modules)
					{
						shader.free(device, allocator);
					}
				}
			};
		}
	}
}