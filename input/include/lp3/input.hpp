#ifndef FILE_LP3_INPUT_HPP
#define FILE_LP3_INPUT_HPP


#include <array>
#include <memory>
#include <string>
#include <vector>
#include <lp3/assert.hpp>
#include <lp3/core.hpp>

#include "input/config.hpp"

namespace lp3 { namespace input {

constexpr int max_button_count = 22;

// --------------------------------------------------------------------
// class Control
// --------------------------------------------------------------------
//     A virtual controller, meaning it can be remapped to different
//     actual controls at runtime. Consists of a series of buttons
//     that have a boolean value and an analog one (which may be 0 or
//     1 if the physical button is digital).
// --------------------------------------------------------------------
LP3_INPUT_API
class Control
{
friend class Controls;

public:
    Control();

    // Returns a value from 0 to 1.
    inline float analog_state(int index) {
		LP3_ASSERT(index >= 0);
		LP3_ASSERT(index < max_button_count);
        return buttons[index];
    }

    // Returns the number of buttons in this control.
    inline int get_button_count()
    {
        return max_button_count;
    }

	// Sets decent defaults that maps controller 0 like so:
	//	up, down, left, right, a, b, x, y, LT, RT, start
	void set_defaults();

    // Returns whether or not the button is pressed at all.
    inline bool state(int index) {
        return analog_state(index) != 0;
    }

protected:
    std::array<float, max_button_count> buttons;
};

// --------------------------------------------------------------------
// PreferedKey
// --------------------------------------------------------------------
//     Helps set up initial control options.
// --------------------------------------------------------------------
LP3_INPUT_API
struct PreferredKey {	
	int key_index;
	const std::string key_name;
};

LP3_INPUT_API
enum class PreferredDevice {
	GAME_PAD = 0,
	KEYBOARD = 1
};

LP3_INPUT_API
struct PreferredButtonMapping {
	PreferredDevice device;
	std::vector<PreferredKey> keys;	

	inline void set_mapping(const char * key) {
		keys.push_back({ lp3::narrow<int>(keys.size()), key });
	}

	template<typename... P>
	void set_mapping(const char * key, P... rest_of_keys) {
		keys.push_back({ lp3::narrow<int>(keys.size()), key });
		set_mapping(rest_of_keys...);
	}	
};


// --------------------------------------------------------------------
// class Controls
// --------------------------------------------------------------------
//     Handles setting up all of the control code, updating the
//     controls each frame, and has utility methods to reconfigure
//     buttons. Also maintains ownership of all Control instances.
// --------------------------------------------------------------------
LP3_INPUT_API
class Controls
{
public:
	Controls();

    ~Controls();

    // Changes a button on a Control to the specified device and key name.
    void configure_button(int control_index,
                          int button_index,
                          const std::string & device_name,
                          const std::string & key_name);

    // Changes the given button on a Control to be whatever is currently
    // being pressed.
    void configure_button_to_current_pressed_key(int control_index,
                                                 int button_index);

	// Returns a reference to a Control. Should not be used after this
	// class is deleted!
	Control & get_control(int control);

	// Returns a list of all currently pressed keys.
	std::vector<std::pair<std::string, std::string>>
		get_current_pressed_keys();

	// Sets the virtual controls to the given key names. Returns false if no
	// mapping could be achieved.
	bool set_defaults(int control_index, 
		              const gsl::span<PreferredButtonMapping> & button_mappings);

    // Call this once each frame.
    void update();
private:
    class ControlsImpl;
    std::unique_ptr<ControlsImpl> impl;
};

} }

#endif
