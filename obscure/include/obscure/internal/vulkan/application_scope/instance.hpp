module;
#include <span>
#include "glfw_vulkan_include.h"
export module obscure.internal.vulkan.instance;

import obscure.version;

namespace obscure
{
	namespace internal
	{
		namespace vulkan
		{
			export struct instance
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
}