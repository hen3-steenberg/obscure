export module obscure.configuration.default_configuration:application;
export import obscure.configuration.application_configuration;

import obscure.configuration.application_configuration;
import obscure.version;

namespace obscure
{
	export namespace configuration
	{
		struct default_application_configuration : application_configuration
		{
			int width, height;
			const char* title;
			version app_version;
			bool resizeable;

			default_application_configuration(int _width, int _height, const char* _title, version ver, bool _resizeable) noexcept;


			virtual int window_width() override;
			virtual int window_height() override;
			virtual const char* application_title() override;
			virtual bool allow_resizing() override;
			virtual version application_version() override;
			virtual ~default_application_configuration() = default;
		};
	}
}