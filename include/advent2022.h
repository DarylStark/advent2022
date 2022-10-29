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
    Advent2022();
    void setup() override;
    void loop() override;
};

#endif // __ADVENT2022_H__