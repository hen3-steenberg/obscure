#ifndef OBSCURE_DEFAULT_CONFIGURATION_DEFINITION
#define OBSCURE_DEFAULT_CONFIGURATION_DEFINITION 1
#include "obscure/configuration/configuration_provider.h"
#include "obscure/vulkan/swap_chain_configuration.h"
#include <memory>
#include <optional>

namespace obscure
{
	namespace configuration
	{
		struct default_application_configuration : application_configuration
		{
			int width, height;
			const char* title;
			version app_version;

			default_application_configuration(int _width, int _height, const char* _title, version ver) noexcept;

			virtual int window_width() override;
			virtual int window_height() override;
			virtual const char* application_title() override;
			virtual version application_version() override;
			virtual ~default_application_configuration() = default;
		};

		struct default_vulkan_configuration : vulkan_configuration
		{
			std::optional<vulkan::swap_chain_configuration> swap_config;
			default_vulkan_configuration(vulkan::application_context const* ctx);

			virtual VkPhysicalDevice pick_device() override;

			virtual VkSwapchainCreateInfoKHR configure_swap_chain() override;

			virtual vulkan::logger_collection vulkan_logging(VkAllocationCallbacks const* allocator = nullptr) override;

			virtual ~default_vulkan_configuration() = default;
		};

		struct default_configuration_provider : configuration_provider
		{
			default_application_configuration app_config;
			std::optional<default_vulkan_configuration> vulkan_config;

			virtual application_configuration* load_application_configuration();

			virtual  vulkan_configuration* load_vulkan_configuration(vulkan::application_context const* ctx);

			default_configuration_provider(int _width, int _height, const char* title, version ver);
		};

		std::unique_ptr<default_configuration_provider> default_configuration(int _width, int _height, const char* title, version ver);

	}
}

#endif