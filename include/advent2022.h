#ifndef __ADVENT2022_H__
#define __ADVENT2022_H__

// STL includes
#include <string>

// Nemo Includes
#include <nemoapp.h>
#include <liquidcrystal.h>

class Advent2022 : public nemo::NemoApp
{
private:
    nemo::LiquidCrystal __display;

public:
    Advent2022() : __display(0x27, 16, 2)
    {
        register_component(__display);
    }

    void setup() override
    {
        // The `_setup` method of the base class will make sure all components
        // are "set-up"
        _setup();

        // Set the welcomes text
        NemoApp::entities["display.line0"] = std::string(" Adventkalender");
        NemoApp::entities["display.line1"] = std::string("      2022");
    }

    void loop() override
    {
        // The `_loop` method of the base class will make sure all components
        // are "looped"
        _loop();

        // Delay :)
        delay(1000);
    }
};

#endif // __ADVENT2022_H__