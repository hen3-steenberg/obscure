module;
#include <array>
#include <atomic>
#include "glfw_vulkan_include.h"
export module obscure.internal.vulkan.frame_data;
import obscure.properties;
import obscure.internal.vulkan.device;
import obscure.internal.vulkan.swap_chain;
import obscure.internal.vulkan.command_pool;
import obscure.internal.vulkan.fence;
import obscure.internal.vulkan.semaphore;
import obscure.internal.vulkan.instance;
import obscure.internal.vulkan.surface;
import obscure.atomic.circular_counter;


namespace obscure
{
	namespace internal
	{
		namespace vulkan
		{
			export struct frame_data
			{
				surface surface;
				device device;
				vulkan::swap_chain swap_chain;
				VkQueue graphics_queue;
				VkQueue present_queue;
				command_pool graphics_command_pool;
				std::array<VkCommandBuffer, obscure::maximum_frames_in_flight()> graphics_command_buffers;
				std::array<fence, obscure::maximum_frames_in_flight()>  draw_complete_fences;
				std::array<semaphore, obscure::maximum_frames_in_flight()>  ready_to_draw_semaphores;
				std::array<semaphore, obscure::maximum_frames_in_flight()>  ready_to_display_semaphores;
				obscure::atomic::circular_counter<obscure::maximum_frames_in_flight()> current_frame;
				std::atomic_bool should_resize;

				uint32_t get_next_frame()&;
				VkCommandBuffer get_current_command_buffer(uint32_t current_frame) const&;

				uint32_t get_next_buffer_index(uint32_t current_frame)&;

				frame_data(VkPhysicalDevice physical_device, vulkan::surface _surface, VkExtent2D native_resolution);
				void free(instance instance) noexcept;

				void wait_for_device_idle()&;
			};
		}
	}
}