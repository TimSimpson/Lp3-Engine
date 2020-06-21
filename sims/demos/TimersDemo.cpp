// ---------------------------------------------------------------------------
// The following example demonstrates using the Timer class to show the real
// world time that has elapsed while an app runs, the GameClock to run code
// using simulated time, and a frame timer for showing FPS.
// ---------------------------------------------------------------------------
#include <lp3/sims.hpp>
#include <lp3/main.hpp>

namespace core = lp3::core;
namespace sims = lp3::sims;

int _main(lp3::main::PlatformLoop & loop) {
    core::LogSystem logger;

    // A real timer.
    sims::Timer timer;

    // A utility for seeing the frames per second.
    sims::FrameTimer frame_timer;

    // Handles running code that updates a game's logic without being
    // susceptible to timing issues from playing on different hardware.
    const std::int64_t ms_per_update = 1000 / 60;  //16 ms for 60 fps
    std::int64_t game_time = 0;
    sims::GameClock clock(ms_per_update);

    const auto result = loop.run([&]() {
        // This simulates the render logic of a game:

        // Update the timer. This is a semi-expensive function (at least
        // according to some Windows docs I read like ten years ago) so it
        // should happen once per frame and cached_time should be used after
        // that.
        timer.get_time();
        LP3_LOG_DEBUG("Game Time: %d\tReal Time: %d\tFPS: %d\tAvg FPS: %d",
                      game_time, timer.cached_time(), frame_timer.get_fps(),
                      frame_timer.get_average_fps());

        clock.run_updates([&game_time](std::int64_t ms) {
            // This is where the game logic will live. Note that this could be
            // called multiple times per frame on fast machines, though it
            // will always happen in the same time slice.
            //
            // The GameClock class figures out how fast the rest of the code
            // is running and how many times to run the game logic in a loop
            // to keep up. Also the GameClock class will never run things too
            // fast, meaning if the game is stopped or paused any internal
            // timers will also be paused.
            //
            // To test this, note how the "game" time and "real" time stay
            // the same until you pause the app somehow. When you return, the
            // "real" time will jump far ahead, while the game time will
            // calmly continue where it left off.
            game_time += ms;
        });

        // The frame timer gives FPS metrics. Call it once each frame.
        frame_timer.next_frame();

        return true;
    });
    return result;
}

LP3_MAIN(_main)
// ~end-doc
