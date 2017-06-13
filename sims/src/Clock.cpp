#define LP3_SIMS_API_CREATE
#include <lp3/sims/Clock.hpp>
#include <chrono>
#include <lp3/log.hpp>

namespace lp3 { namespace sims {

namespace {
    Timer::time_int now_in_ms() {
        auto tp = std::chrono::high_resolution_clock::now();
        return std::chrono::time_point_cast<std::chrono::milliseconds>(tp)
            .time_since_epoch().count();
    }
}

LP3_SIMS_API
Timer::Timer()
:   current_tick_count(0),
    init_high_freq_time(now_in_ms())
{
}

LP3_SIMS_API
Timer::time_int Timer::get_time() {
    current_tick_count = now_in_ms() - init_high_freq_time;
    return current_tick_count;
}

LP3_SIMS_API
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

LP3_SIMS_API
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

LP3_SIMS_API
GameClock::GameClock(Timer::time_int logic_update_time_in_ms)
:   current_time(0),
    lag(0),
    max_lag(std::max(Timer::time_int{500}, logic_update_time_in_ms)),
    update_time_in_ms(logic_update_time_in_ms),
    previous_time(0),
    timer()
{
    previous_time = timer.get_time();
}

LP3_SIMS_API
GameClockRemainder GameClock::get_frame_remainder() const
{
    GameClockRemainder r;
    r.ms = lag;
    r.percent_of_frame = static_cast<double>(lag)
                         / static_cast<double>(update_time_in_ms);
    return r;
}

LP3_SIMS_API
bool GameClock::run_game_logic() {
    if (lag >= update_time_in_ms) {
        lag -= update_time_in_ms;
        return true;
    } else {
        return false;
    }
}


LP3_SIMS_API
std::int64_t GameClock::start_frame() {
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
    if (lag > max_lag) {
        lag = max_lag;
    }

    return lag;
}

}    }
