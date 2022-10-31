#ifndef __BINARYSENSOR_H__
#define __BINARYSENSOR_H__

#include <Arduino.h>
#include "apps/appbase.h"
#include "components/component.h"

namespace viridi
{
    namespace components
    {
        enum input_type
        {
            Normal = 1,
            Pullup = 5,
            Pulldown = 9
        };

        class BinarySensor : public Component
        {
        private:
            uint8_t __gpio;
            input_type __type;

        public:
            BinarySensor(const std::string &name, uint8_t gpio, input_type type = Normal);
            ~BinarySensor();
            void setup() override;
            void loop() override;
        };
    }
};

#endif // __BINARYSENSOR_H__