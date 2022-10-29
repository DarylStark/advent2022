#ifndef __ADVENT2022_H__
#define __ADVENT2022_H__

// STL includes
#include <string>
#include <sstream>
#include <functional>
#include <vector>

// Nemo Includes
#include <nemoapp.h>
#include <liquidcrystal.h>
#include <binarysensor.h>

enum AppMode
{
    Setup = -1,
    MoodLighting = 0,
    Calendar = 1
};

class Advent2022 : public nemo::NemoApp
{
private:
    // The LCD screen
    nemo::LiquidCrystal __display;

    // The buttons
    nemo::BinarySensor __next;
    nemo::BinarySensor __previous;

    // Objects foor the 'moodlighting'
    std::vector<std::string> __moodlightings;

public:
    Advent2022();
    void setup() override;
    void loop() override;

    // Methods to switch between modes
    void configure_mode();
    void set_mood();

    // Methods for the 'moodlighting' mode
    void next_mood(const nemo::EntityEvent &e);
    void previous_mood(const nemo::EntityEvent &e);
};

#endif // __ADVENT2022_H__