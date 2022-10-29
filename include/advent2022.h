#ifndef __ADVENT2022_H__
#define __ADVENT2022_H__

// STL includes
#include <string>

// Nemo Includes
#include <nemoapp.h>
#include <liquidcrystal.h>
#include <binarysensor.h>

#include <iostream>

class Advent2022 : public nemo::NemoApp
{
private:
    // The LCD screen
    nemo::LiquidCrystal __display;

    // The buttons
    nemo::BinarySensor __next;
    nemo::BinarySensor __previous;

public:
    Advent2022() : __display("lcd", 0x27, 16, 2), __next("next", 19, nemo::input_type::Pullup), __previous("previous", 18, nemo::input_type::Pullup)
    {
        register_component(__display);
        register_component(__next);
        register_component(__previous);
    }

    void setup() override
    {
        // The `_setup` method of the base class will make sure all components
        // are "set-up"
        _setup();

        // Set the welcome text
        NemoApp::entities["lcd.display.line0"] = std::string(" Adventkalender");
        NemoApp::entities["lcd.display.line1"] = std::string("      2022");

        // Subscribe to button entities
        NemoApp::entities["next.sensor.low"].subscribe(
            "next_pressed",
            {[](const nemo::EntityEvent &e)
             {
                 if (e.entity)
                 {
                     NemoApp::entities["lcd.display.line1"] = std::string("      NEXT >");
                     return;
                 }
                 NemoApp::entities["lcd.display.line1"] = std::string("      2022");
             }});

        NemoApp::entities["previous.sensor.low"].subscribe(
            "previous_pressed",
            {[](const nemo::EntityEvent &e)
             {
                 if (e.entity)
                 {
                     NemoApp::entities["lcd.display.line1"] = std::string("    < PREV");
                     return;
                 }
                 NemoApp::entities["lcd.display.line1"] = std::string("      2022");
             }});
    }

    void loop() override
    {
        // The `_loop` method of the base class will make sure all components
        // are "looped"
        _loop();

        // Delay :)
        delay(100);
    }
};

#endif // __ADVENT2022_H__