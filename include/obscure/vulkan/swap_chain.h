#ifndef OBSCURE_VULKAN_SWAP_CHAIN_DEFINITION
#define OBSCURE_VULKAN_SWAP_CHAIN_DEFINITION 1

#include "reference.hpp"
#include "device.h"
#include <vector>
namespace obscure
{
	namespace vulkan
	{
		struct swap_chain : public reference<VkSwapchainKHR>
		{
			std::vector<VkImage> swap_chain_images;
			swap_chain(device device);
		};
	}
}
#endif