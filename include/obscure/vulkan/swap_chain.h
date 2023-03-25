#ifndef OBSCURE_VULKAN_SWAP_CHAIN_DEFINITION
#define OBSCURE_VULKAN_SWAP_CHAIN_DEFINITION 1

#include "obscure/vulkan/frame_buffer.h"
#include "obscure/vulkan/render_pass.h"
#include <vector>

namespace obscure
{
	namespace vulkan
	{
		struct swap_chain
		{
			VkSwapchainKHR vk_swap_chain;
			VkFormat image_format;
			VkExtent2D extent;
			render_pass render_pass;
			std::vector<VkImage> swap_chain_images;
			std::vector<image_view> swap_chain_views;
			std::vector<frame_buffer> frame_buffers;
			
			swap_chain() noexcept;
			swap_chain(device device, VkSwapchainCreateInfoKHR create_info, VkAllocationCallbacks const* allocator = nullptr);

			VkSwapchainKHR get_handle() const noexcept;
			void free(device device, VkAllocationCallbacks const* allocator = nullptr) noexcept;
		};
	}
}
#endif