#define LP3_INPUT_API_CREATE
#include "DeviceSource.hpp"
#include <lp3/core.hpp>

inline namespace lp3_input_internal {


DeviceSource::DeviceSource()
:   devices() {
    // Make keyboard as device 0
	devices.push_back(create_sdl_kb());
    // Create devices for all SDL joysticks.
    // If a joystick is a "game controller" use different logic for that.
    LP3_LOG_DEBUG("Iterating SDL joysticks (%i)", SDL_NumJoysticks());
	for (int i = 0; i < SDL_NumJoysticks(); ++i) {
		if (SDL_IsGameController(i)) {
            LP3_LOG_DEBUG("Found a gamepad at index %i.", i);
            gsl::owner<SDL_GameController *> sdl_ctl = SDL_GameControllerOpen(i);
            if (sdl_ctl) {
                devices.push_back(create_sdl_gc(sdl_ctl));
            } else {
                LP3_LOG_WARNING("Error opening controller %i.", i);
            }
		} else {
            LP3_LOG_DEBUG("Found a normal joystick %i.", i);
            gsl::owner<SDL_Joystick *> sdl_js = SDL_JoystickOpen(i);
            if (sdl_js) {
                const char * name = SDL_JoystickNameForIndex(i);
                devices.push_back(create_sdl_js(name ? name : "", sdl_js));
            } else {
                LP3_LOG_WARNING("Error opening joystick %i.", i);
            }
        }
	}

    for (auto & d : devices) {
        LP3_ASSERT(d);
    }
}

Device * DeviceSource::get_device(int index) {
    LP3_ASSERT(index >= 0);
    LP3_ASSERT(index < devices.size());
	return devices[index].get();
}

int DeviceSource::get_device_count() const {
    return lp3::narrow<int>(devices.size());
}

void DeviceSource::update() {
}


}
