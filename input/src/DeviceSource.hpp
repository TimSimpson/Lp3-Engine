#ifndef DEVICE_SOURCE_HPP
#define DEVICE_SOURCE_HPP

#include <vector>
#include <lp3/core.hpp>
#include "Device.hpp"

inline namespace lp3_input_internal {

class Device;

// --------------------------------------------------------------------
// class DeviceSource
// --------------------------------------------------------------------
//     Enumerates and stores collections of devices.
// --------------------------------------------------------------------
class DeviceSource
{
public:
    DeviceSource();

    Device * get_device(int index);

    int get_device_count() const;

    void update();
private:
    std::vector<std::unique_ptr<Device>> devices;
};

}

#endif
