#include <ratio>
#ifndef OBSCURE_STOPWATCH_DEFINITION
#define OBSCURE_STOPWATCH_DEFINITION 1
#include <chrono>

namespace obscure
{
    
    template<typename Rep = double, typename Per = std::ratio<1>>
    struct stopwatch
    {
        using clock = std::chrono::high_resolution_clock;
        using duration_t = std::chrono::duration<Rep, Per>;
        using time_t = std::chrono::time_point<clock, duration_t>;
        
        
        time_t watch_start;
        time_t lap_start;
        stopwatch() noexcept
            : watch_start(clock::now())
        {
            lap_start = watch_start;
        }

        duration_t lap_time() noexcept
        {
            time_t lap_end = clock::now();
            duration_t elapsed = lap_end - lap_start;
            lap_start = lap_end;
            return elapsed;
        }

        duration_t total_time() const noexcept
        {
            return clock::now() - watch_start;
        }
    };
}

#endif