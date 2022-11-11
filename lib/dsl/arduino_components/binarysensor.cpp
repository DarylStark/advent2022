#include "arduino_components/binarysensor.h"

namespace dsl
{
    namespace arduino_components
    {
        BinarySensor::BinarySensor(const std::string &name, uint8_t gpio, input_type type /* = Normal */)
            : Component(name), __gpio(gpio), __type(type)
        {
        }

        BinarySensor::~BinarySensor()
        {
            // Remove all created entities
            entity_manager::entities.erase(_name + ".sensor.low");
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