#define LP3_CORE_API_CREATE
#include <lp3/core/Clock.hpp>
#include <lp3/log.hpp>
#include <lp3/assert.hpp>

namespace lp3 { namespace core {

FrameTimer::FrameTimer(const boost::optional<float> frame_diff_cap)
:   average_fps(60),
    current_tick_count(0),
    frame_count(0),
    fps(0),
    max_frame_diff(frame_diff_cap),
    speed_mod(0),
    start_frame_time(0)
#ifdef LP3_COMPILE_TARGET_WINDOWS
    ,
    init_high_freq_time(0),
    ticks_per_ms(0)
#endif
{
#ifdef LP3_COMPILE_TARGET_WINDOWS
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
    if (ticksPerSecond >= 1000) {
        LP3_LOG_DEBUG("Goldie.");
        ticksPerMS = (Float64)ticksPerSecond / (Float64)1000;
        if (!QueryPerformanceCounter(&li)) {
            useCrapTimer = true;
        }
        else {
            initHighFreqTime = li.QuadPart;
            useCrapTimer = false;
        }
    }
    else {
        useCrapTimer = true;
    }
    LP3_LOG_DEBUG("Init successful.");

    // Seed timer.
    FrameTimer::NextFrame();
    srand((unsigned)GetTickCount());//currentTickCount );
#endif // END LP3_COMPILE_TARGET_WINDOWS
#ifdef LP3_COMPILE_TARGET_DREAMCAST
                                    // I guess one might say that working with the high resolution timer
                                    // in KOS is a bit easier than in Windows.
    startFrameTime = 0;
#endif
}

time_int FrameTimer::next_frame()
{
    return time_int();
}



void initialize_timers() {
    frameCount = 0;
    averageFps = 60;
#ifdef LP3_COMPILE_TARGET_WINDOWS
    /* In Windows, the high resolution timers are nigh infinitely better
    * that the default timer, so we see if we can use those instead.
    * If we can't, we set "useCrapTimer" to true and use the GetTickCount
    * function.  If we can, we mark the initial time that this function is called
    * as "initTime".  The "QueryPerformanceCounter" function returns a timer
    * based in tick counts- to figure out how that relates to actual TIME,
    * we call QueryPerformanceFrequency (which also tells us if High res timers
    * are available), which returns ticks per second.  We convert this to
    * ticksPerMS since thats how the game will see this stuff. */
    std::int64_t ticksPerSecond;
    LARGE_INTEGER li; /* This is a weird data type... */
    LP3_LOG_DEBUG("Trying to call QueryPerformanceFrequency...");
    while (!QueryPerformanceFrequency(&li))
    {
        LP3_LOG_ERROR("Could not get ticksPerSecond`!! Trying again...");
    }
    ticksPerSecond = li.QuadPart;
    LP3_LOG_DEBUG("Successful.");
    if (ticksPerSecond >= 1000) {
        LP3_LOG_DEBUG("Goldie.");
        ticksPerMS = (double) ticksPerSecond / (double) 1000;
        if (!QueryPerformanceCounter(&li)) {
            useCrapTimer = true;
        }
        else {
            initHighFreqTime = li.QuadPart;
            useCrapTimer = false;
        }
    }
    else {
        useCrapTimer = true;
    }
    LP3_LOG_DEBUG("Init successful.");

    // Seed timer.
    next_frame();
    srand((unsigned)GetTickCount());
#endif // END LP3_COMPILE_TARGET_WINDOWS
#ifdef LP3_COMPILE_TARGET_DREAMCAST
                                    // I guess one might say that working with the high resolution timer
                                    // in KOS is a bit easier than in Windows.
    startFrameTime = 0;
#endif
}

clock_time_int next_frame() {
#ifdef LP3_COMPILE_TARGET_WINDOWS
    if (useCrapTimer) { /* Crappy timer. */
        currentTickCount = GetTickCount();
    }
    else {  /* High res timer. */
        LARGE_INTEGER li;
        std::int64_t currentHighFreqTime;
        while (!QueryPerformanceCounter(&li)) {
            LP3_LOG_ERROR("Can't QueryPerformanceCounter "
                "in Clock::NextFrame!");
        }
        currentHighFreqTime = li.QuadPart - initHighFreqTime;
        currentTickCount = (clock_time_int)
            ((double)currentHighFreqTime / ticksPerMS);
    }

#endif // LP3_COMPILE_TARGET_WINDOWS
#ifdef LP3_COMPILE_TARGET_DREAMCAST
    long unsigned int secs;
    long unsigned int msecs;
    timer_ms_gettime(&secs, &msecs);
    currentTickCount = ((ClockTimeInt)secs * 1000) + (ClockTimeInt)msecs;
#endif
    /* Compute time since last call and return it. */
    clock_time_int dif = currentTickCount - startFrameTime;
    startFrameTime = currentTickCount;
    if (maxFrameDiff && dif > maxFrameDiff.get()) // Ensure slow down happens instead of goofy errors.
    {
        fps = 1.0 / ((float)dif / 1000.0f);
        dif = maxFrameDiff.get();
        speedMod = (float)dif / 1000.0f;
    }
    else
    {
        speedMod = (float)dif / 1000.0f;
        fps = 1.0 / speedMod;
    }
    if (frameCount < (3600 - 1))
    {   // If there are always 60 fps, then the average is based over that
        // minute.
        ++frameCount;
    }
    averageFps = ((averageFps * (frameCount - 1)) + fps) / frameCount;
    return dif;
}
}

}    }
