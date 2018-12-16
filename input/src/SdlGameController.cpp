#include "Device.hpp"

inline namespace lp3_input_internal {

constexpr int button_count = 15;
constexpr int shoulder_analogs = 2;
constexpr int stick_sides = 4;

SDL_GameControllerButton button_from_key_code(const int key_code) {
    switch(key_code) {
        case 0:
            return SDL_CONTROLLER_BUTTON_DPAD_UP;
        case 1:
            return SDL_CONTROLLER_BUTTON_DPAD_DOWN;
        case 2:
            return SDL_CONTROLLER_BUTTON_DPAD_LEFT;
        case 3:
            return SDL_CONTROLLER_BUTTON_DPAD_RIGHT;
        case 4:
            return SDL_CONTROLLER_BUTTON_START;
        case 5:
            return SDL_CONTROLLER_BUTTON_BACK;
        case 6:
            return SDL_CONTROLLER_BUTTON_LEFTSTICK;
        case 7:
            return SDL_CONTROLLER_BUTTON_RIGHTSTICK;
        case 8:
            return SDL_CONTROLLER_BUTTON_LEFTSHOULDER;
        case 9:
            return SDL_CONTROLLER_BUTTON_RIGHTSHOULDER;
        case 10:
            return SDL_CONTROLLER_BUTTON_A;
        case 11:
            return SDL_CONTROLLER_BUTTON_B;
        case 12:
            return SDL_CONTROLLER_BUTTON_X;
        case 13:
            return SDL_CONTROLLER_BUTTON_Y;
        case 14:
            return SDL_CONTROLLER_BUTTON_GUIDE;
        default:
            LP3_LOG_ERROR("Bad SDL button code: %i", key_code);
            LP3_THROW2(lp3::core::Exception, "Bad SDL button code.");
            return SDL_CONTROLLER_BUTTON_INVALID;
    }
}

constexpr Sint16 dead_zone = 8000;

inline Sint16 dz_adjust(const Sint16 v) {
    if (v < dead_zone) {
        return 0;
    }
    return v;
}

// Turns SDL value of an axis, which ranges from positive to negative max
// int16 values, to a float which ranges only from 0 to 1, and is zero if
// negative.
float axis_cast_p(const Sint16 v) {
    if (v < dead_zone) {
        return 0.0f;
    }
    return lp3::narrow<float>(v) / 32767.0f;
}

// Like the above, but this time it's zero if positive. If negative it's
// 0 to 1.
float axis_cast_n(const Sint16 v) {
    if (v > (dead_zone * -1)) {
        return 0.0f;
    }
    return lp3::narrow<float>(v) / -32768.0f;
}

bool axis_cast_bool_p(const Sint16 v) {
    return v >= dead_zone;
}

bool axis_cast_bool_n(const Sint16 v) {
    return v < (dead_zone * -1);
}

class SdlGameController : public Device
{
public:
    SdlGameController(gsl::owner<SDL_GameController *> cnt)
    :   controller(cnt) {
    }

    ~SdlGameController() {
        SDL_GameControllerClose(controller);
    }

    // Find first key encountered that is being pressed on this device.
    int find_pressed_key_code() override {
        for (int i = 0; i < get_key_count(); ++ i) {
            if (get_key_state(i)) { return i; }
        }
        return -1;
    }

    // States if control device has analog joystick capabilities at the
    // designated keycode.
    bool is_analog_key(const int key_code) override {
        return key_code >= button_count;
    }

	std::optional<lp3::input::PreferredDevice> get_preferred_device_type() override {
		return lp3::input::PreferredDevice::GAME_PAD;
	}

    // Name of the Device.
    const char * get_name() const override {
        return "SDL Game Controller";
    }

    // Number of keys on a device.
    int get_key_count() override {
        // 15 "buttons", 2 triggers, two analog sticks.
        return button_count + shoulder_analogs + stick_sides + stick_sides;
    }

    // Name of a key on a device.
    const char * get_key_name(const int key_code) override {
        LP3_ASSERT(key_code >= 0);
        LP3_ASSERT(key_code < 25);
        static const char * KEY_NAMES[] = {
            "D-pad Up",
            "D-pad Down",
            "D-pad Left",
            "D-pad Right",
            "Start",
            "Back",
            "Left Analog Depress",
            "Right Analog Depress",
            "LB",
            "RB",
            "A",
            "B",
            "X",
            "Y",
            "GUIDE",
            "LT",
            "RT",
            "Left Analog Up",
            "Left Analog Down",
            "Left Analog Left",
            "Left Analog Right",
            "Right Analog Up",
            "Right Analog Down",
            "Right Analog Left",
            "Right Analog Right",
        };
        return KEY_NAMES[key_code];
    }

    // For boolean buttons, returns whether or not a key was pressed.
    bool get_key_state(const int key_code) override {
        if (key_code < button_count) {
            SDL_GameControllerButton btn = button_from_key_code(key_code);
            auto result = SDL_GameControllerGetButton(controller, btn);
            return 0 != result;
        }
        switch(key_code) {
            case button_count + 0:
                return axis_cast_bool_p(SDL_GameControllerGetAxis(
                    controller, SDL_CONTROLLER_AXIS_TRIGGERLEFT));
            case button_count + 1:
                return axis_cast_bool_p(SDL_GameControllerGetAxis(
                    controller, SDL_CONTROLLER_AXIS_TRIGGERRIGHT));
            case button_count + 2:
                return axis_cast_bool_n(SDL_GameControllerGetAxis(
                    controller, SDL_CONTROLLER_AXIS_LEFTY));
            case button_count + 3:
                return axis_cast_bool_p(SDL_GameControllerGetAxis(
                    controller, SDL_CONTROLLER_AXIS_LEFTY));
            case button_count + 4:
                return axis_cast_bool_n(SDL_GameControllerGetAxis(
                    controller, SDL_CONTROLLER_AXIS_LEFTX));
            case button_count + 5:
                return axis_cast_bool_p(SDL_GameControllerGetAxis(
                    controller, SDL_CONTROLLER_AXIS_LEFTX));
            case button_count + 6:
                return axis_cast_bool_n(SDL_GameControllerGetAxis(
                    controller, SDL_CONTROLLER_AXIS_RIGHTY));
            case button_count + 7:
                return axis_cast_bool_p(SDL_GameControllerGetAxis(
                    controller, SDL_CONTROLLER_AXIS_RIGHTY));
            case button_count + 8:
                return axis_cast_bool_n(SDL_GameControllerGetAxis(
                    controller, SDL_CONTROLLER_AXIS_RIGHTX));
            case button_count + 9:
                return axis_cast_bool_p(SDL_GameControllerGetAxis(
                    controller, SDL_CONTROLLER_AXIS_RIGHTX));
            default:
                LP3_LOG_ERROR("Bad SDL axis key code: %i", key_code)
                LP3_THROW2(lp3::core::Exception, "Bad SDL axis key code.");
                return 0;
        }
    }

    // For analog keys, gets a floating point number representing how
    // intensely a key is held down.
    float get_key_analog_state(const int key_code) override {
        if (key_code < button_count) {
            SDL_GameControllerButton btn = button_from_key_code(key_code);
            auto result = SDL_GameControllerGetButton(controller, btn);
            return 0 != result ? 1.0f : 0.0f;
        }
        switch(key_code) {
            case button_count + 0:
                return axis_cast_p(SDL_GameControllerGetAxis(
                    controller, SDL_CONTROLLER_AXIS_TRIGGERLEFT));
            case button_count + 1:
                return axis_cast_p(SDL_GameControllerGetAxis(
                    controller, SDL_CONTROLLER_AXIS_TRIGGERRIGHT));
            case button_count + 2:
                return axis_cast_n(SDL_GameControllerGetAxis(
                    controller, SDL_CONTROLLER_AXIS_LEFTY));
            case button_count + 3:
                return axis_cast_p(SDL_GameControllerGetAxis(
                    controller, SDL_CONTROLLER_AXIS_LEFTY));
            case button_count + 4:
                return axis_cast_n(SDL_GameControllerGetAxis(
                    controller, SDL_CONTROLLER_AXIS_LEFTX));
            case button_count + 5:
                return axis_cast_p(SDL_GameControllerGetAxis(
                    controller, SDL_CONTROLLER_AXIS_LEFTX));
            case button_count + 6:
                return axis_cast_n(SDL_GameControllerGetAxis(
                    controller, SDL_CONTROLLER_AXIS_RIGHTY));
            case button_count + 7:
                return axis_cast_p(SDL_GameControllerGetAxis(
                    controller, SDL_CONTROLLER_AXIS_RIGHTY));
            case button_count + 8:
                return axis_cast_n(SDL_GameControllerGetAxis(
                    controller, SDL_CONTROLLER_AXIS_RIGHTX));
            case button_count + 9:
                return axis_cast_p(SDL_GameControllerGetAxis(
                    controller, SDL_CONTROLLER_AXIS_RIGHTY));
            default:
                LP3_LOG_ERROR("Bad SDL axis key code: %i", key_code)
                LP3_THROW2(lp3::core::Exception, "Bad SDL axis key code.");
                return 0;
        }
    }

    // Updates the device. This must be called once per frame.
    void update() override {
        // SDL_GameControllerUpdate can be called, but isn't necessary
        // if the SDL event loop is being used.
    }

private:
    gsl::owner<SDL_GameController *> controller;
};

std::unique_ptr<Device> create_sdl_gc(gsl::owner<SDL_GameController *> cnt) {
    LP3_ASSERT(cnt);
    std::unique_ptr<Device> d{new SdlGameController(cnt)};
    return d;
}

}
