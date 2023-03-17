#include "obscure/color/virtual_terminal.h"

#ifdef WIN32
#define ENABLE_VIRTUAL_TERMINAL true
#include <Windows.h>
#else
#define ENABLE_VIRTUAL_TERMINAL false
#endif // WIN

void obscure::virtual_terminal::enable_virtual_terminal()
{
	if constexpr (ENABLE_VIRTUAL_TERMINAL)
	{
		auto out_handle = GetStdHandle(STD_OUTPUT_HANDLE);
		DWORD mode;
		GetConsoleMode(out_handle, &mode);

		mode = mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING;
		SetConsoleMode(out_handle, mode);
	}
}