#ifndef OBSCURE_CONFIGURATION_PROVIDER_DEFINITION
#define OBSCURE_CONFIGURATION_PROVIDER_DEFINITION 1
#include "obscure/configuration/application_configuration.h"
#include "obscure/configuration/vulkan_configuration.h"
namespace obscure
{
	namespace configuration
	{
		struct configuration_provider
		{
			virtual application_configuration* load_application_configuration() = 0;

			virtual  vulkan_configuration* load_vulkan_configuration() = 0;

			virtual ~configuration_provider() = default;
		};
	}
}
#endif