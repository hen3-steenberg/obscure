#ifndef OBSCURE_CONFIGURATION_CONTAINER_DEFINITION
#define OBSCURE_CONFIGURATION_CONTAINER_DEFINITION 1
#include "obscure/configuration/configuration_provider.h"
#include <concepts>

namespace obscure
{
	namespace configuration
	{
		template<typename T>
		concept configuration_container = requires(T t)
		{
			{t.get()} -> std::convertible_to<configuration_provider*>;
		};
	}
}
#endif
