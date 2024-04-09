#ifndef OBSCURE_APPLICATION_PROPERTY_DEFINITIONS
#define OBSCURE_APPLICATION_PROPERTY_DEFINITIONS 1
#include "obscure/version.hpp"

namespace obscure
{
	[[nodiscard]] consteval int window_height() noexcept
	{
		return 0;//native resolution
	}

	[[nodiscard]] consteval int window_width() noexcept
	{
		return 0;//native resolution
	}

	[[nodiscard]] consteval const char* application_name() noexcept
	{
		return "OBSCURE_APPLICATION";
	}

	[[nodiscard]] consteval version application_version()
	{
		return version{ 1,0,0 };
	}
}

#endif