#ifndef __WS2812B_H__
#define __WS2812B_H__

#include <Adafruit_NeoPixel.h>
#include "components/component.h"
#include "entity_manager/entity.h"

namespace viridi
{
    namespace components
    {
        // Class for colors
        class Color
        {
        public:
            uint8_t red;
            uint8_t green;
            uint8_t blue;
            float brightness = 1.f;

            operator uint32_t() const;
        };

        class WS2812B : public Component
        {
        private:
            uint8_t __gpio;
            uint16_t __length;
            Adafruit_NeoPixel __component;

        public:
            WS2812B(const std::string &name, uint8_t gpio, uint16_t length);
            ~WS2812B();
            void setup() override;
            void loop() override;

            // LED strip methods
            void fill(const viridi::entity_manager::EntityEvent &e);
            void clear();
            void show();
            void set_color(uint16_t start_led, uint16_t count, Color color, bool show = false);
        };
    }
};

#endif // __WS2812B_H__