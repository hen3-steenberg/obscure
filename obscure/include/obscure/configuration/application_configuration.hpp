export module obscure.configuration.application_configuration;
import obscure.version;

namespace obscure
{
	namespace configuration
	{
		export struct application_configuration
		{
			virtual int window_width() = 0;
			virtual int window_height() = 0;
			virtual bool allow_resizing() = 0;
			virtual const char* application_title() = 0;
			virtual version application_version() = 0;
			virtual ~application_configuration() = default;
		};
	}
}