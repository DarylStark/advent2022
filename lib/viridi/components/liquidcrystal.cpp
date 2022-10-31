#include "components/liquidcrystal.h"

namespace viridi
{
    namespace components
    {
        // Public methods for LiquidCrystal
        LiquidCrystal::LiquidCrystal(const std::string &name, uint8_t address, uint8_t width, uint8_t height) : Component(name), __address(address), __width(width), __height(height), __component(__address, __width, __height)
        {
        }

        void LiquidCrystal::setup()
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
                entity_manager::entities[name.str()] = std::string();

                // Subscribe to the entity
                entity_manager::entities[name.str()].subscribe(
                    "set_text",
                    {std::bind(&LiquidCrystal::set_text, this, i, std::placeholders::_1)});
            }

            // Create entities for the backlight
            entity_manager::entities[_name + ".display.backlight"] = true;
            entity_manager::entities[_name + ".display.backlight"].subscribe(
                "set_backlight",
                {std::bind(&LiquidCrystal::set_backlight, this, std::placeholders::_1)});
        }

        void LiquidCrystal::loop()
        {
        }

        void LiquidCrystal::clear(int line /* = -1*/)
        {
            if (line == -1)
            {
                __component.clear();
                return;
            }
            __component.setCursor(0, line);
            __component.print(std::string(__width, ' ').c_str());
        }

        void LiquidCrystal::set_text(int line, const viridi::entity_manager::EntityEvent &e)
        {
            const viridi::entity_manager::Entity &text = e.entity;
            clear(line);
            __component.setCursor(0, line);
            __component.print(static_cast<std::string>(text).c_str());
        }

        void LiquidCrystal::set_backlight(const viridi::entity_manager::EntityEvent &e)
        {
            if (e.entity)
            {
                __component.backlight();
                return;
            }
            __component.noBacklight();
        }
    };
};