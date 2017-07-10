#include "Device.hpp"

inline namespace lp3_input_internal {

class SdlKeyboard : public Device
{
public:
    SdlKeyboard() {
        update();
    }

    ~SdlKeyboard() {
    }

    // Find first key encountered that is being pressed on this device.
    int find_pressed_key_code() override {
        for (int i = 0; i < get_key_count(); ++ i) {
            if (get_key_state(i)) { return i; }
        }
        return -1;
    }

    bool is_analog_key(const int key_code) override {
        (void)key_code;  // unused
        return false;
    }

	boost::optional<lp3::input::PreferredDevice> get_preferred_device_type() override {
		return lp3::input::PreferredDevice::KEYBOARD;
	}

    const char * get_name() const override {
        return "Keyboard";
    }

    int get_key_count() override {
        return num_keys;
    }

    const char * get_key_name(const int key_code) override {
        LP3_ASSERT(key_code >= 0);
        LP3_ASSERT(key_code < num_keys);
        const char * name = SDL_GetScancodeName(static_cast<SDL_Scancode>(key_code));
        // TODO: Do I need to free this pointer? The docs aren't clear.
        return name;
    }

    // For boolean buttons, returns whether or not a key was pressed.
    bool get_key_state(const int key_code) override {
        const SDL_Scancode sdl_code = static_cast<SDL_Scancode>(key_code);
        return 0 != state[sdl_code];
    }

    // For analog keys, gets a floating point number representing how
    // intensely a key is held down.
    float get_key_analog_state(const int key_code) override {
        const SDL_Scancode sdl_code = static_cast<SDL_Scancode>(key_code);
        return state[sdl_code] ? 1.0f : 0.0f;
    }

    // Updates the device. This must be called once per frame.
    void update() override {
        this->state = SDL_GetKeyboardState(&num_keys);
    }

private:
    const Uint8 * state;
    int num_keys;
};

std::unique_ptr<Device> create_sdl_kb() {
    std::unique_ptr<Device> d{new SdlKeyboard()};
    return d;
}

}
