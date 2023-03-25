#ifndef OBSCURE_VULKAN_SWAP_CHAIN_DEFINITION
#define OBSCURE_VULKAN_SWAP_CHAIN_DEFINITION 1

#include "obscure/vulkan/image_view.h"
#include <vector>

namespace obscure
{
	namespace vulkan
	{
		struct swap_chain
		{
			VkSwapchainKHR vk_swap_chain;
			std::vector<VkImage> swap_chain_images;
			std::vector<image_view> swap_chain_views;
			VkFormat image_format;
			VkExtent2D extent;
			swap_chain() noexcept;
			swap_chain(device device, VkSwapchainCreateInfoKHR create_info, VkAllocationCallbacks const* allocator = nullptr);

			VkSwapchainKHR get_handle() const noexcept;
			void free(device device, VkAllocationCallbacks const* allocator = nullptr) noexcept;
		};
	}
}
#endif