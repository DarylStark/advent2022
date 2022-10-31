#include "components/binarysensor.h"

namespace viridi
{
    namespace components
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
            entity_manager::entities[_name + ".sensor.low"] = digitalRead(__gpio) == LOW;
        }

        void BinarySensor::loop()
        {
            entity_manager::entities[_name + ".sensor.low"] = digitalRead(__gpio) == LOW;
        }
    };
};