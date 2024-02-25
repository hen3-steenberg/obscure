module;

module obscure.configuration.default_configuration:application;
import obscure.version;

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