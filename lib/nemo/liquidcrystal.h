#ifndef __LIQUIDCRYSTAL_H__
#define __LIQUIDCRYSTAL_H__

#include <sstream>
#include <string>
#include <LiquidCrystal_I2C.h>
#include "nemoapp.h"
#include "component.h"

namespace nemo
{
    class LiquidCrystal : public Component
    {
    private:
        uint8_t __address;
        uint8_t __width;
        uint8_t __height;
        LiquidCrystal_I2C __component;

    public:
        LiquidCrystal(const std::string &name, uint8_t address, uint8_t width, uint8_t height) : Component(name), __address(address), __width(width), __height(height), __component(__address, __width, __height)
        {
        }

        void setup() override
        {
            // Initialize the component
            __component.init();
            __component.backlight();

            // Clear the screen
            clear();

            // Create entities for the text
            for (int i = 0; i < __height; i++)
            {
                // Create the entity name
                std::stringstream name;
                name << _name + ".display.line" << i;

                // Create the entity
                NemoApp::entities[name.str()] = std::string();

                // Subscribe to the entity
                NemoApp::entities[name.str()].subscribe(
                    "set_text",
                    {std::bind(&LiquidCrystal::set_text, this, i, std::placeholders::_1)});
            }

            // Create entities for the backlight
            NemoApp::entities[_name + ".display.backlight"] = true;
            NemoApp::entities[_name + ".display.backlight"].subscribe(
                "set_backlight",
                {std::bind(&LiquidCrystal::set_backlight, this, std::placeholders::_1)});
        }

        void loop() override {}

        void clear(int line = -1)
        {
            if (line == -1)
            {
                __component.clear();
                return;
            }
            __component.setCursor(0, line);
            __component.print(std::string(__width, ' ').c_str());
        }

        void set_text(int line, const nemo::EntityEvent &e)
        {
            const nemo::Entity &text = e.entity;
            clear(line);
            __component.setCursor(0, line);
            __component.print(static_cast<std::string>(text).c_str());
        }

        void set_backlight(const nemo::EntityEvent &e)
        {
            if (e.entity)
            {
                __component.backlight();
                return;
            }
            __component.noBacklight();
        }
    };
}

#endif // __LIQUIDCRYSTAL_H__