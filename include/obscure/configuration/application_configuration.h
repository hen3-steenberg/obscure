#ifndef OBSCURE_APPLICATION_CONFIGURATION_DEFINITION
#define OBSCURE_APPLICATION_CONFIGURATION_DEFINITION 1
#include "obscure/utilities/version.hpp"
namespace obscure
{
	namespace configuration
	{
		struct application_configuration
		{
			virtual int window_width() = 0;
			virtual int window_height() = 0;
			virtual const char* application_title() = 0;
			virtual version application_version() = 0;
			virtual ~application_configuration() = default;
		};
	}
}
#endif