#ifndef OBSCURE_VIRTUAL_TERMINAL_DEFINITIONS
#define OBSCURE_VIRTUAL_TERMINAL_DEFINITIONS 1

namespace obscure
{
	namespace virtual_terminal
	{
		/// <summary>
		/// On windows sets the console window to render virtual terminal sequences, on linux this is the default behaviour and this function does nothing.
		/// </summary>
		inline void enable_virtual_terminal();

		constexpr const char clear[] = "\x01b[0m";

		constexpr const char black_text[] = "\x01b[30m";
		constexpr const char red_text[] = "\x01b[31m";
		constexpr const char green_text[] = "\x01b[32m";
		constexpr const char yellow_text[] = "\x01b[33m";
		constexpr const char blue_text[] = "\x01b[34m";
		constexpr const char magenta_text[] = "\x01b[35m";
		constexpr const char cyan_text[] = "\x01b[36m";
		constexpr const char white_text[] = "\x01b[37m";

		constexpr const char grey_text[] = "\x01b[90m";
		constexpr const char bright_red_text[] = "\x01b[91m";
		constexpr const char bright_green_text[] = "\x01b[92m";
		constexpr const char bright_yellow_text[] = "\x01b[93m";
		constexpr const char bright_blue_text[] = "\x01b[94m";
		constexpr const char bright_magenta_text[] = "\x01b[95m";
		constexpr const char bright_cyan_text[] = "\x01b[96m";
		constexpr const char bright_white_text[] = "\x01b[97m";

		constexpr const char black_background[] = "\x01b[40m";
		constexpr const char red_background[] = "\x01b[41m";
		constexpr const char green_background[] = "\x01b[42m";
		constexpr const char yellow_background[] = "\x01b[43m";
		constexpr const char blue_background[] = "\x01b[44m";
		constexpr const char magenta_background[] = "\x01b[45m";
		constexpr const char cyan_background[] = "\x01b[46m";
		constexpr const char white_background[] = "\x01b[47m";

		constexpr const char grey_background[] = "\x01b[100m";
		constexpr const char bright_red_background[] = "\x01b[101m";
		constexpr const char bright_green_background[] = "\x01b[102m";
		constexpr const char bright_yellow_background[] = "\x01b[103m";
		constexpr const char bright_blue_background[] = "\x01b[104m";
		constexpr const char bright_magenta_background[] = "\x01b[105m";
		constexpr const char bright_cyan_background[] = "\x01b[106m";
		constexpr const char bright_white_background[] = "\x01b[107m";
	}
}
#endif // !