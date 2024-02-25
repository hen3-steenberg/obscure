module;
#include <chrono>
module obscure.stopwatch;

obscure::stopwatch::stopwatch() noexcept
	: start_time(stopwatch::clock::now()), frame_start(start_time) {}

obscure::stopwatch::seconds obscure::stopwatch::frame_time() noexcept
{
	time_point now = clock::now();
	seconds result = now - frame_start;
	frame_start = now;
	return result;
}

void obscure::stopwatch::reset() noexcept
{
	start_time = clock::now();
	frame_start = start_time;
}

obscure::stopwatch::seconds obscure::stopwatch::elapsed_time() const noexcept
{
	return clock::now() - start_time;
}