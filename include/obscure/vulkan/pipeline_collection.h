#ifndef OBSCURE_VULKAN_PIPELINE_COLLECTION_DEFINITION
#define OBSCURE_VULKAN_PIPELINE_COLLECTION_DEFINITION 1
#include "obscure/vulkan/shader_module.h"
#include "obscure/vulkan/pipeline_layout.h"
#include "obscure/vulkan/render_pass.h"
#include "obscure/vulkan/frame_buffer.h"
#include <vector>


namespace obscure
{
	namespace vulkan
	{
		struct application_context;

		struct pipeline_collection
		{
			std::vector<shader_module> shaders;


			pipeline_layout static_triangle_layout;


			union
			{
				struct
				{
					VkPipeline static_triangle_pipeline;
				};
				VkPipeline pipelines[1];
			};
			
			pipeline_collection();
			pipeline_collection(obscure::vulkan::application_context const* context, VkAllocationCallbacks const* allocator = nullptr);

			void free(device device, VkAllocationCallbacks const* allocator = nullptr) noexcept;
		};
	}
}

#endif