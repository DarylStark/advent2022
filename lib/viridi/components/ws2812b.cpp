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
            entity_manager::entities.erase(_name + ".color");
            entity_manager::entities.erase(_name + ".filled");
        }

        void WS2812B::setup()
        {
            // Configure the strip
            __component.setPin(__gpio);
            __component.begin();
            __component.updateLength(__length);

            // Register entities
            entity_manager::entities[_name + ".color"] = 0x00000000;
            entity_manager::entities[_name + ".filled"] = false;

            // Add handlers to the entities
            entity_manager::entities[_name + ".filled"].subscribe(
                "fill",
                {std::bind(&WS2812B::fill, this, std::placeholders::_1)});
        }

        void WS2812B::loop()
        {
        }

        void WS2812B::fill(const viridi::entity_manager::EntityEvent &e)
        {
            if (e.entity)
            {
                __component.fill(static_cast<int>(entity_manager::entities[_name + ".color"]));
                show();
                return;
            }
            clear();
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