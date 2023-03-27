#ifndef _DEBUG
#define ENABLE_VALIDATION_LAYERS false
#elif _DEBUG
#define ENABLE_VALIDATION_LAYERS true
#else
#define ENABLE_VALIDATION_LAYERS false
#endif

#ifndef OBSCURE_PROPERTIES_DEFINITION
#define OBSCURE_PROPERTIES_DEFINITION 1
#include "obscure/utilities/version.hpp"

namespace obscure
{
	consteval bool enable_vulkan_validation_layers()
	{
		return ENABLE_VALIDATION_LAYERS;
	}

	consteval obscure::version obscure_version()
	{
		return obscure::version{ 1,0,0 };
	}

	consteval size_t maximum_frames_in_flight()
	{
		return 2;
	}
}

#endif