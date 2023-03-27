#include "obscure/configuration/default_configuration.h"
#include "obscure/vulkan/device_picker.h"
#include "obscure/properties.hpp"


VkPhysicalDevice obscure::configuration::default_vulkan_configuration::pick_device(vulkan::instance instance, vulkan::surface surface)
{
	obscure::vulkan::device_picker picker(instance, surface);
	return picker.pick_device();
}

obscure::vulkan::logger_collection obscure::configuration::default_vulkan_configuration::vulkan_logging(vulkan::instance instance, VkAllocationCallbacks const* allocator)
{
	if constexpr (obscure::enable_vulkan_validation_layers())
	{
		return obscure::vulkan::logger_collection
		{
			new obscure::vulkan::verbose_console_logger(instance, allocator),
			new obscure::vulkan::verbose_csv_logger(instance, "vulkan_log.csv", allocator)
		};
	}
	else
	{
		return obscure::vulkan::logger_collection{};
	}
}

obscure::configuration::default_application_configuration::default_application_configuration(int _width, int _height, const char* _title, version ver, bool _resizable) noexcept
	: width(_width), height(_height), title(_title), app_version(ver), resizeable(_resizable)
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

bool obscure::configuration::default_application_configuration::allow_resizing()
{
	return resizeable;
}
obscure::version obscure::configuration::default_application_configuration::application_version()
{
	return app_version;
}

obscure::configuration::default_configuration_provider::default_configuration_provider(int _width, int _height, const char* title, version ver, bool resizable)
	: app_config(_width, _height, title, ver, resizable), vulkan_config()
{}

obscure::configuration::application_configuration* obscure::configuration::default_configuration_provider::load_application_configuration()
{
	return &app_config;
}

obscure::configuration::vulkan_configuration* obscure::configuration::default_configuration_provider::load_vulkan_configuration()
{
	return &vulkan_config;
}

std::unique_ptr<obscure::configuration::default_configuration_provider> obscure::configuration::default_configuration(int _width, int _height, const char* title, obscure::version ver, bool resizeable)
{
	return std::make_unique<obscure::configuration::default_configuration_provider>(_width, _height, title, ver, resizeable);
}