#include "obscure/configuration/default_configuration.h"
#include "obscure/vulkan/device_picker.h"
#include "obscure/properties.hpp"

obscure::configuration::default_vulkan_configuration::default_vulkan_configuration(obscure::vulkan::application_context const* ctx)
	: vulkan_configuration(ctx), swap_config(std::nullopt)
{}

VkPhysicalDevice obscure::configuration::default_vulkan_configuration::pick_device()
{
	obscure::vulkan::device_picker picker(context->instance, context->surface);
	return picker.pick_device();
}

VkSwapchainCreateInfoKHR obscure::configuration::default_vulkan_configuration::configure_swap_chain()
{
	if (!swap_config.has_value())
	{
		swap_config = obscure::vulkan::swap_chain_configuration{ context };
	}
	return swap_config.value().parse_configuration();
}

obscure::vulkan::logger_collection obscure::configuration::default_vulkan_configuration::vulkan_logging(VkAllocationCallbacks const* allocator)
{
	if constexpr (obscure::enable_vulkan_validation_layers())
	{
		return obscure::vulkan::logger_collection
		{
			new obscure::vulkan::verbose_console_logger(context->instance, allocator),
			new obscure::vulkan::verbose_csv_logger(context->instance, "vulkan_log.csv", allocator)
		};
	}
	else
	{
		return obscure::vulkan::logger_collection{};
	}
}

obscure::configuration::default_application_configuration::default_application_configuration(int _width, int _height, const char* _title, version ver) noexcept
	: width(_width), height(_height), title(_title), app_version(ver)
{}

int obscure::configuration::default_application_configuration::window_width()
{
	return width;
}
int obscure::configuration::default_application_configuration::window_height()
{
	return height;
}
const char* obscure::configuration::default_application_configuration::application_title()
{
	return title;
}
obscure::version obscure::configuration::default_application_configuration::application_version()
{
	return app_version;
}

obscure::configuration::default_configuration_provider::default_configuration_provider(int _width, int _height, const char* title, version ver)
	: app_config(_width, _height, title, ver), vulkan_config(std::nullopt)
{}

obscure::configuration::application_configuration* obscure::configuration::default_configuration_provider::load_application_configuration()
{
	return &app_config;
}

obscure::configuration::vulkan_configuration* obscure::configuration::default_configuration_provider::load_vulkan_configuration(obscure::vulkan::application_context const* ctx)
{
	if (!vulkan_config.has_value())
	{
		vulkan_config = obscure::configuration::default_vulkan_configuration(ctx);
	}
	return &vulkan_config.value();
}

std::unique_ptr<obscure::configuration::default_configuration_provider> obscure::configuration::default_configuration(int _width, int _height, const char* title, obscure::version ver)
{
	return std::make_unique<obscure::configuration::default_configuration_provider>(_width, _height, title, ver);
}