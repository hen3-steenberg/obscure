module;

module obscure.configuration.default_configuration;

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

obscure::configuration::configuration_provider* obscure::configuration::default_configuration_provider::get()
{
	return this;
}

obscure::configuration::default_configuration_provider obscure::configuration::default_configuration(int _width, int _height, const char* title, obscure::version ver, bool resizeable)
{
	return obscure::configuration::default_configuration_provider(_width, _height, title, ver, resizeable);
}