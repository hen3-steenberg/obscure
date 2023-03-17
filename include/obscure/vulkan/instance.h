#ifndef OBSCURE_VULKAN_INSTANCE_DEFINITION
#define OBSCURE_VULKAN_INSTANCE_DEFINITION 1

#include <memory>
#include <span>
#include "glfw_vulkan_include.h"
#include "obscure/utilities/version.hpp"

namespace obscure
{
	namespace vulkan
	{
		struct instance : private std::shared_ptr<VkInstance_T>
		{
			static std::span<const char*> GetAvailableLayers();

			static std::span<const char*> GetRequiredDeviceExtensions();

			instance(const char* AppName, version AppVersion);

			VkInstance get_instance() const noexcept;
		};
	}
}
#endif