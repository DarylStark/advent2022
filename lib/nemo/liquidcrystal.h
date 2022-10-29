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
        LiquidCrystal(const std::string &name, uint8_t address, uint8_t width, uint8_t height);
        void setup() override;
        void loop() override;
        void clear(int line = -1);
        void set_text(int line, const nemo::EntityEvent &e);
        void set_backlight(const nemo::EntityEvent &e);
    };
}

#endif // __LIQUIDCRYSTAL_H__