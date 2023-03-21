#ifndef OBSCURE_VULKAN_SWAP_CHAIN_DEFINITION
#define OBSCURE_VULKAN_SWAP_CHAIN_DEFINITION 1

#include "reference.hpp"
#include "image_view.h"
#include <vector>
namespace obscure
{
	namespace vulkan
	{
		struct swap_chain : public reference<VkSwapchainKHR>
		{
			std::vector<VkImage> swap_chain_images;
			std::vector<image_view> swap_chain_views;
			VkFormat image_format;
			VkExtent2D extent;

			swap_chain(device device);
		};
	}
}
#endif