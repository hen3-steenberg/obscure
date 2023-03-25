#ifndef OBSCURE_VULKAN_RENDER_CONTEXT_DEFINITION
#define OBSCURE_VULKAN_RENDER_CONTEXT_DEFINITION 1
#include "obscure/vulkan/application_context.h"

namespace obscure
{
	namespace vulkan
	{
		struct render_context
		{
		protected:
			VkCommandBuffer command_buffer;
			pipeline_collection const& pipelines;
			VkExtent2D extent;
		public:
			render_context(VkCommandBuffer _command_buffer, frame_buffer buffer, application_context const* context);
			render_context& draw_static_triangle();
			void end_rendering();
		};
	}
}

#endif