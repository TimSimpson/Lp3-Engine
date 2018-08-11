#ifndef FILE_LP3INPUTINTERNAL_DEVICE_HPP
#define FILE_LP3INPUTINTERNAL_DEVICE_HPP

#include <memory>
#include <lp3/core.hpp>
#include <lp3/input.hpp>

inline namespace lp3_input_internal {

// --------------------------------------------------------------------
// class Device
// --------------------------------------------------------------------
//     A control device represents a physical means of input.
//     Devices and keycodes are mapped to a player action, such as
//     "JUMP".  This class offers a common interface for devices from
//     DirectInput and XInput to the Dreamcast controller.
// --------------------------------------------------------------------
class Device
{
public:
    virtual ~Device() = default;

    // Find first key encountered that is being pressed on this device.
    virtual int find_pressed_key_code() = 0;

    // States if control device has analog joystick capabilities at the
    // designated keycode.
    virtual bool is_analog_key(const int key_code) = 0;

	virtual boost::optional<lp3::input::PreferredDevice> get_preferred_device_type() {
		return boost::none;
	}

	virtual bool is_keyboard() {
		return false;
	}

    // Name of the Device.
    virtual const char * get_name() const = 0;

    // Number of keys on a device.
    virtual int get_key_count() = 0;

    // Name of a key on a device.
    virtual const char * get_key_name(const int key_code) = 0;

    // For boolean buttons, returns whether or not a key was pressed.
    virtual bool get_key_state(const int key_code) = 0;

    // For analog keys, gets a floating point number representing how
    // intensely a key is held down.
    virtual
        float get_key_analog_state(const int key_code) = 0;

    // Updates the device. This must be called once per frame.
    virtual void update() = 0;
};


std::unique_ptr<Device> create_sdl_gc(gsl::owner<SDL_GameController *> cnt);

std::unique_ptr<Device> create_sdl_js(const char * name,
                                      gsl::owner<SDL_Joystick *> cnt);

std::unique_ptr<Device> create_sdl_kb();

}

#endif
