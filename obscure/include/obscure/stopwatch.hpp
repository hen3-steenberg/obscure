#include <chrono>
export module obscure.stopwatch;

namespace obscure
{
	export struct stopwatch
	{
		typedef std::chrono::duration<double> seconds;
		typedef std::chrono::high_resolution_clock clock;
		typedef std::chrono::time_point<clock, seconds> time_point;
	private:
		time_point start_time;
		time_point frame_start;

	public:
		stopwatch() noexcept;
		seconds frame_time() noexcept;
		void reset() noexcept;
		seconds elapsed_time() const noexcept;
	};
}