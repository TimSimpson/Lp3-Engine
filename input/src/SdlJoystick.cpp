#include "Device.hpp"

inline namespace lp3_input_internal {

constexpr int axis_sides = 2;

constexpr int hat_sides = 4;

constexpr float diag_slop = 0.7071067811865476f;

constexpr Sint16 dead_zone = 8000;

inline Sint16 dz_adjust(const Sint16 v) {
    if (v < 8000) {
        return 0;
    }
	return v;
}

inline Sint16 n_dz_adjust(const Sint16 v) {
    if (v > -8000) {
        return 0;
    }
	return v;
}


class SdlJoystick : public Device
{
public:
    SdlJoystick(const char * name, gsl::owner<SDL_Joystick *> js)
    :   name(name),
        key_names(),
        joystick(js),
        axis_count(SDL_JoystickNumAxes(js)),
        button_count(SDL_JoystickNumButtons(js)),
        hat_count(SDL_JoystickNumHats(js))
    {
        std::string prefix = "Button ";
        for (int i = 0; i < this->button_count; ++i) {
            key_names.push_back(str(boost::format("Button %i") % i));
        }
        for (int i = 0; i < this->hat_count; ++i) {
            key_names.push_back(str(boost::format("Hat %i Up") % i));
            key_names.push_back(str(boost::format("Hat %i Down") % i));
            key_names.push_back(str(boost::format("Hat %i Left") % i));
            key_names.push_back(str(boost::format("Hat %i Right") % i));
        }
        for (int i = 0; i < this->axis_count; ++i) {
            key_names.push_back(str(boost::format("Axis %i Negative") % i));
            key_names.push_back(str(boost::format("Axis %i Positive") % i));
        }
        for (const auto & s : key_names) {
            LP3_LOG_DEBUG("   [%s]", s);
        }
    }

    ~SdlJoystick() {
        SDL_JoystickClose(joystick);
    }

    int find_pressed_key_code() override {
        for (int i = 0; i < get_key_count(); ++ i) {
            if (get_key_state(i)) { return i; }
        }
        return -1;
    }

    bool is_analog_key(const int key_code) override {
        return this->button_count + (this->hat_count * hat_sides) <= key_code;
    }

    const char * get_name() const override {
        return name.c_str();
    }

    int get_key_count() override {
        return (axis_count * axis_sides) + button_count + (hat_count * hat_sides);
    }

    const char * get_key_name(const int key_code) override {
        LP3_ASSERT(key_code >= 0);
        LP3_ASSERT(key_code < get_key_count());
        return key_names[key_code].c_str();
    }

    bool get_key_state(const int key_code) override {
        return get_key_analog_state(key_code) > 0.0f;
    }

    float get_key_analog_state(const int key_code) override {
        LP3_ASSERT(key_code >= 0);
        LP3_ASSERT(key_code < get_key_count());
        if (key_code < this->button_count) {
            return 1 == SDL_JoystickGetButton(joystick, key_code) ? 1.0f : 0.0f;
        } else if (key_code < this->button_count + (this->hat_count * hat_sides)) {
            const int subtracted_kc = key_code - this->button_count;
            const int position_check = subtracted_kc % 4;
            const int hat_code = subtracted_kc / 4;
            Uint8 r = SDL_JoystickGetHat(joystick, hat_code);
            switch(position_check) {
                case 0:
                    if (SDL_HAT_UP == r) {
                        return 1.0f;
                    } else if (SDL_HAT_LEFTUP == r || SDL_HAT_RIGHTUP == r) {
                        return diag_slop;
                    }
                    return 0.0f;
                case 1:
                    if (SDL_HAT_DOWN == r) {
                        return 1.0f;
                    } else if (SDL_HAT_LEFTDOWN == r || SDL_HAT_RIGHTDOWN == r) {
                        return diag_slop;
                    }
                    return 0.0f;
                case 2:
                    if (SDL_HAT_LEFT == r) {
                        return 1.0f;
                    } else if (SDL_HAT_LEFTUP == r || SDL_HAT_LEFTDOWN == r) {
                        return diag_slop;
                    }
                    return 0.0f;
                case 3:
                    if (SDL_HAT_RIGHT == r) {
                        return 1.0f;
                    } else if (SDL_HAT_RIGHTUP == r || SDL_HAT_RIGHTDOWN == r) {
                        return diag_slop;
                    }
                    return 0.0f;
				default:
					LP3_THROW2(lp3::core::Exception, "Bad case!");
            }
        } else {
            const int subtracted_kc
                = key_code - (button_count + (hat_sides * hat_count));
            const int axis_code = subtracted_kc / axis_sides;

            const Sint16 r = SDL_JoystickGetAxis(joystick, axis_code);
            //LP3_LOG_DEBUG("MARIO, %i is %i", axis_code, r);
            const int position_check = subtracted_kc % axis_sides;
            if (0 == position_check) {
                return lp3::narrow<float>(n_dz_adjust(r)) / -32768.0f;
            } else {
                return lp3::narrow<float>(dz_adjust(r)) / 32767.0f;
            }
        }

    }

    void update() override {
        // SDL_JoystickUpdate can be called, but isn't necessary
        // if the SDL event loop is being used.
    }

private:
    std::string name;
    std::vector<std::string> key_names;
    gsl::owner<SDL_Joystick *> joystick;
    const int axis_count;
    const int button_count;

    const int hat_count;

};

std::unique_ptr<Device> create_sdl_js(const char * name,
                                      gsl::owner<SDL_Joystick *> js) {
    LP3_ASSERT(js);
    std::unique_ptr<Device> d{new SdlJoystick(name, js)};
    return d;
}

}
