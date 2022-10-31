#include "components/ws2812b.h"

namespace viridi
{
    namespace components
    {
        // Public methods for Color
        Color::operator uint32_t() const
        {
            uint32_t rv = red * brightness;
            rv = rv << 8;
            rv += green * brightness;
            rv = rv << 8;
            rv += blue * brightness;
            return rv;
        }

        // Public methods for LiquidCrystal
        WS2812B::WS2812B(const std::string &name, uint8_t gpio, uint16_t length)
            : Component(name), __gpio(gpio), __length(length)
        {
        }

        WS2812B::~WS2812B()
        {
        }

        void WS2812B::setup()
        {
            // Configure the strip
            __component.setPin(__gpio);
            __component.begin();
            __component.updateLength(__length);

            // TODO: register entities
        }

        void WS2812B::loop()
        {
        }

        void WS2812B::clear()
        {
            __component.clear();
        }

        void WS2812B::show()
        {
            __component.show();
        }

        void WS2812B::set_color(uint16_t start_led, uint16_t count, Color color, bool show /* = false */)
        {
            uint32_t color_code = static_cast<uint32_t>(color);
            for (uint16_t led = start_led; led < start_led + count; led++)
            {
                __component.setPixelColor(led, color_code);
            }
            if (show)
                this->show();
        }
    };
};