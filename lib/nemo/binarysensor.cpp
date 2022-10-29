#include "binarysensor.h"

namespace nemo
{
    BinarySensor::BinarySensor(const std::string &name, uint8_t gpio, input_type type /* = Normal */)
        : Component(name), __gpio(gpio), __type(type)
    {
    }

    void BinarySensor::setup()
    {
        // Configure the GPIO
        pinMode(__gpio, __type);

        // Create the entities
        NemoApp::entities[_name + ".sensor.low"] = digitalRead(__gpio) == LOW;
    }

    void BinarySensor::loop()
    {
        NemoApp::entities[_name + ".sensor.low"] = digitalRead(__gpio) == LOW;
    }
};