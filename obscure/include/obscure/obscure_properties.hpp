#ifndef OBSCURE_PROPERTY_DEFINITIONS
#define OBSCURE_PROPERTY_DEFINITIONS 1
#include "version.hpp"
namespace obscure
{
    [[nodiscard]] consteval version obscure_version()
	{
		return version {1, 0, 0};
	}

    [[nodiscard]] consteval bool enable_debug_validation()
    {
        #ifdef DEBUG
            return true;
        #else
            return false;
        #endif
    }

    [[nodiscard]] consteval const char* obscure_name()
    {
        return "OBSCURE";
    }
}
#endif