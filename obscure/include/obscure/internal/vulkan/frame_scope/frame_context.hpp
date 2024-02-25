module;
#include "glfw_vulkan_include.h"
export module obscure.internal.vulkan.frame_context;
import obscure.internal.vulkan.frame_data;

namespace obscure
{
	namespace internal
	{
		namespace vulkan
		{
			struct frame_context 
			{
				frame_data & data;
				uint32_t current_frame;
				uint32_t current_buffer_index;
				
				VkCommandBuffer command_buffer() const&;
				VkExtent2D extent() const&;
				

				frame_context(frame_data& _data);

				frame_context(frame_context const& other) = delete;
				frame_context(frame_context&& other) noexcept;


			};
		}
	}
}