#define LP3_CORE_API_CREATE
#include <lp3/core/Clock.hpp>
#include <lp3/log.hpp>
#include <lp3/assert.hpp>
#include <windows.h>

namespace lp3 { namespace core {

LP3_CORE_API
Timer::Timer()
:   current_tick_count(0),
    init_high_freq_time(0),
    ticks_per_ms(0)
{
    /* In Windows, the high resolution timers are nigh infinitely better
    * that the default timer, so we see if we can use those instead.
    * If we can't, we set "useCrapTimer" to true and use the GetTickCount
    * function.  If we can, we mark the initial time that this function is called
    * as "initTime".  The "QueryPerformanceCounter" function returns a timer
    * based in tick counts- to figure out how that relates to actual TIME,
    * we call QueryPerformanceFrequency (which also tells us if High res timers
    * are available), which returns ticks per second.  We convert this to
    * ticksPerMS since thats how the game will see this stuff. */
    std::uint64_t ticksPerSecond;
    LARGE_INTEGER li; /* This is a weird data type... */
    LP3_LOG_DEBUG("Trying to call QueryPerformanceFrequency...");
    while (!QueryPerformanceFrequency(&li))
    {
        LP3_LOG_ERROR("Could not get ticksPerSecond`!! Trying again...");
    }
    ticksPerSecond = li.QuadPart;
    LP3_LOG_DEBUG("Successful.");

    LP3_ASSERT_TRUE(ticksPerSecond >= 1000);
    this->ticks_per_ms = (double)ticksPerSecond / (double)1000;
    auto result = QueryPerformanceCounter(&li);
    LP3_ASSERT_TRUE(result);
    this->init_high_freq_time = li.QuadPart;

    LP3_LOG_DEBUG("Init successful.");
}

LP3_CORE_API
Timer::time_int Timer::get_time() {
    LARGE_INTEGER li;
    std::int64_t currentHighFreqTime;
    while (!QueryPerformanceCounter(&li)) {
        LP3_LOG_ERROR("Can't QueryPerformanceCounter "
            "in Clock::NextFrame!");
    }
    currentHighFreqTime = li.QuadPart - init_high_freq_time;
    current_tick_count = (time_int)
        ((double)currentHighFreqTime / ticks_per_ms);
    
    return current_tick_count;
}

LP3_CORE_API
FrameTimer::FrameTimer(const boost::optional<float> frame_diff_cap)
:   average_fps(60),
    frame_count(0),
    fps(0),
    max_frame_diff(frame_diff_cap),
    speed_mod(0),
    start_frame_time(0),
    timer()
{    
    // Seed timer.
    this->next_frame();    
}

LP3_CORE_API
std::int64_t FrameTimer::next_frame() {
    const auto current_tick_count = timer.get_time();

    /* Compute time since last call and return it. */
    std::int64_t dif = current_tick_count - start_frame_time;
    start_frame_time = current_tick_count;
    if (max_frame_diff && dif > max_frame_diff.get()) { // Ensure slow down happens instead of goofy errors.
        fps = 1.0f / ((float)dif / 1000.0f);
        dif = static_cast<int>(max_frame_diff.get());
        speed_mod = (float)dif / 1000.0f;
    } else {
        speed_mod = (float)dif / 1000.0f;
        fps = 1.0f / speed_mod;
    }
    if (frame_count < (3600 - 1)) {
        // If there are always 60 fps, then the average is based over that
        // minute.
        ++frame_count;
    }
    average_fps = ((average_fps * (frame_count - 1)) + fps) / frame_count;
    return dif;
}

LP3_CORE_API
SimulatorStopWatch::SimulatorStopWatch(Timer::time_int logic_update_time_in_ms)
:   current_time(0),
    lag(0),
    update_time_in_ms(logic_update_time_in_ms),
    previous_time(0),
    timer()
{
    previous_time = timer.get_time();
}

LP3_CORE_API
SimulatedStopWatchRemainder SimulatorStopWatch::get_frame_remainder() const
{
    SimulatedStopWatchRemainder r;
    r.ms = lag;
    r.percent_of_frame = static_cast<double>(lag) 
                         / static_cast<double>(update_time_in_ms);
    return r;
}

LP3_CORE_API
bool SimulatorStopWatch::run_game_logic() {
    if (lag >= update_time_in_ms) {
        lag -= update_time_in_ms;
        return true;
    } else {
        return false;
    }
}


LP3_CORE_API
std::int64_t SimulatorStopWatch::start_frame() {
    current_time = timer.get_time();
    const auto elapsed = current_time - previous_time;
    previous_time = current_time;
    lag += elapsed;

    // If the render loop starts to drag, we can hit a situation where we 
    // may have to simulate an unseemly amount of time. So we cap the elapsed
    // time we will be simulating at around half a second - meaning that if
    // the real loop can't go at least that fast, the game will noticably
    // slow down.  However it beats the alternative, which would make the
    // app increasingly unresponsive.
    if (lag > 500) {
        lag = 500;
    }    

    return lag;
}

}    }
