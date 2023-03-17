#ifndef OBSCURE_STOPWATCH_DEFINITION
#define OBSCURE_STOPWATCH_DEFINITION 1
#include <chrono>

namespace obscure
{
	struct stopwatch
	{
		typedef std::chrono::duration<double> seconds;
		typedef std::chrono::high_resolution_clock clock;
		typedef std::chrono::time_point<clock, seconds> time_point;
	private:
		time_point last_reset;
	public:
		stopwatch() noexcept;
		seconds lap_time() noexcept;
		void reset() noexcept;
		seconds elapsed_time() const noexcept;
	};
}

#endif