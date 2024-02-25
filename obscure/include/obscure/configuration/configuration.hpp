module;
#include <concepts>
export module obscure.configuration;
export import obscure.configuration.application_configuration;
export import obscure.configuration.vulkan_configuration;
export import obscure.configuration.configuration_provider;

namespace obscure
{
	namespace configuration
	{
		export
		template<typename T>
		concept configuration_container = requires(T t)
		{
			{t.get()} -> std::convertible_to<configuration_provider*>;
			{t.operator->()} -> std::convertible_to<configuration_provider*>;
		};
	}
}