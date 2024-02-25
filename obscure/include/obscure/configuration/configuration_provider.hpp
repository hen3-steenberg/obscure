export module obscure.configuration.configuration_provider;
import obscure.configuration.application_configuration;
import obscure.configuration.vulkan_configuration;

namespace obscure
{
	namespace configuration
	{
		export struct configuration_provider
		{
			virtual application_configuration* load_application_configuration() = 0;

			virtual  vulkan_configuration* load_vulkan_configuration() = 0;

			virtual ~configuration_provider() = default;
		};
	}
}