
#ifndef _DEBUG
#define ENABLE_DEBUG_FEATURES false
#elif _DEBUG
#define ENABLE_DEBUG_FEATURES true
#else
#define ENABLE_DEBUG_FEATURES false
#endif

export module obscure.properties;
import obscure.version;

namespace obscure
{
	export consteval bool enable_debug_features()
	{
		return ENABLE_DEBUG_FEATURES;
	}

	export consteval obscure::version obscure_version()
	{
		return obscure::version{ 1,0,0 };
	}

	export consteval size_t maximum_frames_in_flight()
	{
		return 2;
	}
}
