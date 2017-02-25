#ifndef FILE_LP3_CORE_CLOCK_HPP
#define FILE_LP3_CORE_CLOCK_HPP

#include <cstdint>
#include <boost/optional.hpp>
#include "config.hpp"

namespace lp3 { namespace core {

using clock_time_int = std::int64_t;

// ----------------------------------------------------------------------------
// class Timer
// ----------------------------------------------------------------------------
//     Represents the real, outside world time.
// ----------------------------------------------------------------------------
LP3_CORE_API
class Timer {
public:
    using time_int = std::int64_t;

    Timer();
       
    // Returns a value that increments by milliseconds
    inline time_int cached_time() const {
        return current_tick_count;
    }

    // Signals the next frame.  Must be called once per frame.
    //
    // This actually updates the value in "currentTickCount" and returns the
    // time difference between when it was called last and now. We store
    // "currentTickCount" because accessing it a bunch causes slow downs.
    time_int get_time();

private:
    time_int current_tick_count;    
#ifdef LP3_COMPILE_TARGET_WINDOWS
    time_int init_high_freq_time;   // Time at initialization
    double ticks_per_ms;  // High resolution ticks per MS.
#endif
};


// ----------------------------------------------------------------------------
// class FrameTimer
// ----------------------------------------------------------------------------
//     Designed to time the frame of a game loop to learn FPS.
// ----------------------------------------------------------------------------
LP3_CORE_API
class FrameTimer {
public:
    FrameTimer(const boost::optional<float> frame_diff_cap = boost::none);

    inline float get_average_fps() const {
        return average_fps;
    }

    inline float get_fps() const {
        return fps;
    }

    inline float get_speed_mod() const {
        return speed_mod;
    }

    // Returns a value that increments by milliseconds
    inline std::int64_t cached_time() const {
        return timer.cached_time();
    }

    // Signals the next frame.  Must be called once per frame.
    //
    // This actually updates the value in "currentTickCount" and returns the
    // time difference between when it was called last and now. We store
    // "currentTickCount" because accessing it a bunch causes slow downs.
    std::int64_t next_frame();

private:
    float average_fps;              
    int frame_count;                
    float fps;                      
    // The maximum difference between the start of one frame and the
    // next for the purposes of game logic.
    // For example, this keeps the "GetSpeedMod()" float from returning
    // enormous values if the game is paused in Windows.
    boost::optional<float> max_frame_diff;
    float speed_mod;                 // % of a second it took for last frame.
    std::int64_t start_frame_time;
    Timer timer;
};

// ----------------------------------------------------------------------------
// class SimulatorStopWatchRemainder
// ----------------------------------------------------------------------------
//     The amount of time left unsimulated by the SimulatedStopWatch's
//     run_updates method.
// ----------------------------------------------------------------------------
LP3_CORE_API
struct SimulatedStopWatchRemainder {
    // The number of milliseconds that was not accounted for during 
    // run_updates.
    std::int64_t ms;
    // The percentage of a "logical frame" that was not accounted for.
    double percent_of_frame;
};

// ----------------------------------------------------------------------------
// class SimulatorStopWatch
// ----------------------------------------------------------------------------
//     A way to model a game's main loop that avoids pitfalls with timing.
//
//     Typical solutions involve using a magical double that all numbers can
//     be multiplied by, which represents the percentage of a second a frame
//     takes.
//
//     However this means that the game logic (stuff like physics or AI) 
//     becomes non-deterministic and harder to replicate. So instead the
//     pattern is to program certain critical systems so that they always 
//     update for a given number of milliseconds (similar to games that are
//     locked to framerates) and then run those routines repeatedly until a
//     game catches up to the elapsed time.
//
//     This implies your code has a structure like this:
//
//     StopWatch w(16);
//     while(true) {
//         input.update();
//         auto r = w.run_updates([&](std::int64_t ms) {  // ms is always 16
//             move_characters(ms);
//             view.animate(ms);
//         }
//         old_state = gfx.animation  
//         gfx.animate(r.ms);  // Update the animation by the tiny extra bit
//         gfx.update();
//         gfx.animation = old_state  // restore so we can update the animation
//                                    // again in run_updates.
//     }
// ----------------------------------------------------------------------------
LP3_CORE_API
class SimulatorStopWatch {
public:
    // Accepts a number, which is the time in milliseconds that each call to
    // your game's update loop will process. 
    SimulatorStopWatch(std::int64_t update_time_in_ms);     

    template<typename F>
    SimulatedStopWatchRemainder run_updates(F f) {
        start_frame();
        while (run_game_logic()) {
            f(update_time_in_ms);
        }
        return get_frame_remainder();
    }
    
private:
    SimulatedStopWatchRemainder get_frame_remainder() const;

    // Call this repeatedly in a loop until it returns false, each time making
    // a call to update the game logic in the body of the loop. The call 
    // should act on the time period specified in this class's constructor (so 
    // if you pass in 15, then you should process logical entities in the game 
    // world for 15 ms). 
    bool run_game_logic();
    
    // Called once per frame in the main loop of your app. Returns the elapsed
    // time.
    std::int64_t start_frame();
private:
    std::int64_t current_time;
    std::int64_t lag;
    std::int64_t update_time_in_ms;
    std::int64_t previous_time;
    Timer timer;    
};

}   }


#endif
