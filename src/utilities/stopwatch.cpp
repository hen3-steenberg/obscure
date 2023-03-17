#include "obscure/utilities/stopwatch.h"

obscure::stopwatch::stopwatch() noexcept
	: last_reset(stopwatch::clock::now()) {}

obscure::stopwatch::seconds obscure::stopwatch::lap_time() noexcept
{
	time_point now = clock::now();
	seconds result = now - last_reset;
	last_reset = now;
	return result;
}

void obscure::stopwatch::reset() noexcept
{
	last_reset = clock::now();
}

obscure::stopwatch::seconds obscure::stopwatch::elapsed_time() const noexcept
{
	return clock::now() - last_reset;
}