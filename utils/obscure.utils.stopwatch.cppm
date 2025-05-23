module;
#include <ratio>
#include <chrono>
export module obscure.utils.stopwatch;

export namespace obscure
{
    template<typename Rep = double, typename Per = std::chrono::seconds::period>
    struct stopwatch
    {
        using clock = std::chrono::high_resolution_clock;
        using duration_t = std::chrono::duration<Rep, Per>;
        using time_t = clock::time_point;

        static time_t now() noexcept {
            return clock::now();
        }


        time_t watch_start;
        time_t lap_start;
        stopwatch() noexcept
            : watch_start(now()), lap_start(watch_start)
        {}

        duration_t lap_time() noexcept
        {
            time_t lap_end = now();
            duration_t elapsed = duration_t{lap_end - lap_start};
            lap_start = lap_end;
            return elapsed;
        }

        duration_t total_time() const noexcept
        {
            return duration_t{now() - watch_start};
        }
    };
}