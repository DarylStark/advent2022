#ifndef __ADVENT2022_H__
#define __ADVENT2022_H__

// STL includes
#include <string>
#include <sstream>
#include <functional>
#include <vector>

// C includes
#include <time.h>

// Nemo Includes
#include <nemoapp.h>
#include <liquidcrystal.h>
#include <binarysensor.h>

// Arduino includes
#include <WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

enum AppMode
{
    Setup = -1,
    MoodLighting = 0,
    Calendar = 1
};

struct Date
{
    uint16_t year;
    uint16_t month;
    uint16_t day;
};

class Advent2022 : public nemo::NemoApp
{
private:
    // The LCD screen
    nemo::LiquidCrystal __display;

    // Objects for NTP
    WiFiUDP __udp;
    NTPClient __ntp_client;
    unsigned long __ntp_last_update;

    // The buttons
    nemo::BinarySensor __next;
    nemo::BinarySensor __previous;

    // Objects foor the 'moodlighting'
    std::vector<std::string> __moodlightings;

public:
    Advent2022();
    void setup() override;
    void loop() override;

    // Methods for the WiFi connection
    void reconnect_to_wifi();
    void update_ntp(bool force = false);

    // Methods for the date
    Date get_date();

    // Methods to switch between modes
    void configure_mode();
    void set_mood();

    // Methods for the 'moodlighting' mode
    void next_mood(const nemo::EntityEvent &e);
    void previous_mood(const nemo::EntityEvent &e);

    // Methods for the 'calendar' mode
    void next_index(const nemo::EntityEvent &e);
    void previous_index(const nemo::EntityEvent &e);
};

#endif // __ADVENT2022_H__