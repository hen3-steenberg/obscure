#ifndef OBSCURE_VULKAN_INSTANCE_DEFINITION
#define OBSCURE_VULKAN_INSTANCE_DEFINITION 1

#include <span>
#include "obscure/vulkan/glfw_vulkan_include.h"
#include "obscure/utilities/version.hpp"

namespace obscure
{
	namespace vulkan
	{
		struct instance
		{
			static std::span<const char*> GetAvailableLayers();

			static std::span<const char*> GetRequiredDeviceExtensions();

			VkInstance vk_instance;

			instance() noexcept;

			instance(const char* AppName, version AppVersion, VkAllocationCallbacks const* allocator = nullptr);

			void free(VkAllocationCallbacks const* allocator = nullptr) noexcept;

			VkInstance get_instance() const noexcept;
			
		};
	}
}
#endif