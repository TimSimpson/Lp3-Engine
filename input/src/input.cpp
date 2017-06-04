#define LP3_INPUT_API_CREATE
#include <lp3/input.hpp>
#include "Device.hpp"
#include "DeviceSource.hpp"

using namespace lp3::input;

namespace {

    // Reference to a key on a device.
    struct ButtonMapping
    {
        Device * device;
        std::string device_name;
        int key_index;

        ButtonMapping()
        :   device(nullptr),
            device_name{},
            key_index(0)
        {}

        explicit operator bool() const {
            return device != nullptr;
        }

        inline float get_state()
        {
            return device->get_key_analog_state(key_index);
        }
    };

    using ControlConfigs =
        std::array<ButtonMapping, max_button_count>;

    constexpr int controller_count = 4;

    struct ControlMapping
    {
        std::array<ButtonMapping, max_button_count> buttons;
        Control control;

        ControlMapping()
        :   buttons{},
            control()
        {}
	};
}

// --------------------------------------------------------------------
// class Control
// --------------------------------------------------------------------
LP3_INPUT_API
Control::Control()
:   buttons{}
{}

// --------------------------------------------------------------------
// class Controls::ControlsImpl
// --------------------------------------------------------------------

class Controls::ControlsImpl
{
public:
    ControlsImpl()
    :   device_source(),
        mapping{}
    {
        setDefaults();
    }

    void configure_button(int control_index,
                          int button_index,
                          Device * const device,
                          int key_index) {
        LP3_ASSERT(control_index >= 0 && control_index < controller_count);
        LP3_ASSERT(button_index >= 0 && button_index < max_button_count);
        ButtonMapping & bm = mapping[control_index].buttons[button_index];
        bm.device = device;
        bm.device_name = device->get_name();
        bm.key_index = key_index;
    }

    void configure_button(int control_index,
                          int button_index,
                          const std::string & deviceName,
                          const std::string & keyName) {
        //Control & control = controls[controlIndex];
        for (int index = 0;
             index < device_source.get_device_count();
             ++ index)
        {
            Device * const device = device_source.get_device(index);
            if (deviceName == device->get_name())
            {
                for (int keyIndex = 0;
                     keyIndex < device->get_key_count(); ++ keyIndex)
                {
                    const char * const name = device->get_key_name(keyIndex);
                    if (keyName == name)
                    {
                        configure_button(control_index, button_index,
                                         device, keyIndex);
                        return;
                    }
                }
            }
        }
        LP3_LOG_ERROR("Can't configure button: device name %s / key "
                      "name %s not found!", deviceName, keyName);
        LP3_THROW2(lp3::core::Exception, "Bad arguments to configure button.");
    }

    void configure_button_to_current_pressed_key(int control_index,
                                                 int button_index) {
        for (int index = 0; index < device_source.get_device_count();
             ++ index)
        {
            Device * const device = device_source.get_device(index);
            const int key_index = device->find_pressed_key_code();
            if (key_index >= 0)
            {
                configure_button(control_index, button_index, device,
                                 key_index);
                return;
            }
        }
    }

    Control & get_control(const int control_index) {
        LP3_ASSERT(control_index >= 0 && control_index < controller_count);
        return mapping[control_index].control;
    }

	std::vector<std::pair<std::string, std::string>>
	get_current_pressed_keys() {
		std::vector<std::pair<std::string, std::string>> result;
		for (int index = 0; index < device_source.get_device_count();
			++index)
		{
			Device * const device = device_source.get_device(index);
            LP3_ASSERT(device);
            for (int key_index = 0; key_index < device->get_key_count();
                 ++ key_index) {
                if (device->get_key_state(key_index)) {
                    result.push_back({device->get_name(),
                                      device->get_key_name(key_index)});
                }
            }
		}
		return result;
	}

    // Sets defaults to use a basic game pad (directions, face buttons, start)
    void setDefaults() {
		// TODO: Make this pick the first SDL Controller, since that's
		//       always some nice XInput style setup option
        LP3_LOG_DEBUG("Setting defaults.");

            //if (this->device_source->get_device_count() <= 0) {
            //    LP3_LOG_DEBUG("No device returned? Aborting default init.");
            //    return;
            //}
            //const auto x_device_index = static_cast<WindowsDeviceSource *>(
            //    this->device_source.get())->find_xinput_device();
            //if (!x_device_index)  // Use the stinky old keyboard.
            //{
            //    const Device * const device = this->device_source->get_device(0);
            //    const std::string device_name(device->get_name());
            //    configure_button(0,  0, device_name, "up arrow            ");
            //    configure_button(0,  1, device_name, "down arrow          ");
            //    configure_button(0,  2, device_name, "left arrow          ");
            //    configure_button(0,  3, device_name, "right arrow         ");
            //    configure_button(0,  4, device_name, "S                   ");
            //    configure_button(0,  5, device_name, "D                   ");
            //    configure_button(0,  6, device_name, "A                   ");
            //    configure_button(0,  7, device_name, "W                   ");
            //    configure_button(0,  8, device_name, "Q                   ");
            //    configure_button(0,  9, device_name, "E                   ");
            //    configure_button(0, 10, device_name, "R                   ");
            //}
            //else  // Use the controller.
            //{
            //    const Device * const device =
            //        this->device_source->get_device(x_device_index.get());
            //    const std::string deviceName(device->get_name());
            //    configure_button(0,  0, deviceName, "Left Analog Up");
            //    configure_button(0,  1, deviceName, "Left Analog Down");
            //    configure_button(0,  2, deviceName, "Left Analog Left");
            //    configure_button(0,  3, deviceName, "Left Analog Right");
            //    configure_button(0,  4, deviceName, "A");
            //    configure_button(0,  5, deviceName, "B");
            //    configure_button(0,  6, deviceName, "X");
            //    configure_button(0,  7, deviceName, "Y");
            //    configure_button(0,  8, deviceName, "LT");
            //    configure_button(0,  9, deviceName, "RT");
            //    configure_button(0, 10, deviceName, "Start");
            //}

    }

    void update() {
        // Calls to the actual code responsible for getting gamepad and other
        // input from various platform specific APIs.
        device_source.update();
        // Now iterate the Control instances and fill them with the current
        // data.
        for (ControlMapping & cm : mapping) {
            for (int i = 0; i < max_button_count; ++ i) {
                ButtonMapping & bm = cm.buttons[i];
                if (bm) {
                    cm.control.buttons[i] = bm.get_state();
                }
            }
        }
    }

private:
	DeviceSource device_source;
	std::array<ControlMapping, controller_count> mapping;
};

// --------------------------------------------------------------------
// class Controls::Controls
// --------------------------------------------------------------------
LP3_INPUT_API
Controls::Controls()
: impl(new ControlsImpl())
{
}

LP3_INPUT_API
Controls::~Controls()
{}

LP3_INPUT_API
void Controls::configure_button(int control_index,
                                int button_index,
                                const std::string & device_name,
                                const std::string & key_name)
{
    impl->configure_button(control_index, button_index, device_name, key_name);
}

LP3_INPUT_API
void Controls::configure_button_to_current_pressed_key(
        int control_index, int button_index) {
    impl->configure_button_to_current_pressed_key(control_index, button_index);
}

LP3_INPUT_API
Control & Controls::get_control(int control) {
	return impl->get_control(control);
}

LP3_INPUT_API
std::vector<std::pair<std::string, std::string>>
Controls::get_current_pressed_keys() {
	return impl->get_current_pressed_keys();
}

// Call this once per frame to update devices.
LP3_INPUT_API
void Controls::update() {
    impl->update();
}
