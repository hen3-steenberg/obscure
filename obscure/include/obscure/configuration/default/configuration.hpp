export module obscure.configuration.default_configuration;
export import :application;
export import :vulkan;

import obscure.configuration.configuration_provider;
import obscure.version;

namespace obscure
{
	namespace configuration
	{
		export struct default_configuration_provider : configuration_provider
		{
			default_application_configuration app_config;
			default_vulkan_configuration vulkan_config;

			virtual application_configuration* load_application_configuration();

			virtual vulkan_configuration* load_vulkan_configuration();

			default_configuration_provider(int _width, int _height, const char* title, version ver, bool resizeable)
				: app_config(_width, _height, title, ver, resizable), vulkan_config()
			{}

			configuration_provider* get();
		};

		export default_configuration_provider default_configuration(int _width, int _height, const char* title, version ver, bool resizeable);
	}
}

obscure::configuration::application_configuration* obscure::configuration::default_configuration_provider::load_application_configuration()
{
	return &app_config;
}

obscure::configuration::vulkan_configuration* obscure::configuration::default_configuration_provider::load_vulkan_configuration()
{
	return &vulkan_config;
}

obscure::configuration::configuration_provider* obscure::configuration::default_configuration_provider::get()
{
	return this;
}

obscure::configuration::default_configuration_provider obscure::configuration::default_configuration(int _width, int _height, const char* title, obscure::version ver, bool resizeable)
{
	return obscure::configuration::default_configuration_provider(_width, _height, title, ver, resizeable);
}
