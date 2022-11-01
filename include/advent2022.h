#ifndef __ADVENT2022_H__
#define __ADVENT2022_H__

// STL includes
#include <string>
#include <sstream>
#include <functional>
#include <vector>

// C includes
#include <time.h>

// Viridi includes
#include <apps/appbase.h>
#include <components/liquidcrystal.h>
#include <components/binarysensor.h>
#include <components/ws2812b.h>

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

struct Mood
{
    std::string name;
    std::vector<uint16_t> leds;
};

class Advent2022 : public viridi::apps::AppBase
{
private:
    // The LCD screen
    viridi::components::LiquidCrystal __display;

    // Objects for NTP
    WiFiUDP __udp;
    NTPClient __ntp_client;
    unsigned long __ntp_last_update;

    // The buttons
    viridi::components::BinarySensor __next;
    viridi::components::BinarySensor __previous;
    viridi::components::BinarySensor __mode;
    viridi::components::BinarySensor __enter;

    // LED strip
    viridi::components::WS2812B __leds;

    // Objects foor the 'moodlighting'
    std::vector<Mood> __moodlightings;

public:
    Advent2022();
    void setup() override;
    void loop() override;

    // Methods for the WiFi connection
    bool reconnect_to_wifi();
    void disconnect_wifi();
    void update_ntp(bool force = false);

    // Methods for the date
    Date get_date();

    // Methods to switch between modes
    void next_mode_button(const viridi::entity_manager::EntityEvent &e);
    void configure_mode();

    // Methods for the 'moodlighting' mode
    void set_mood();
    void next_mood(const viridi::entity_manager::EntityEvent &e);
    void previous_mood(const viridi::entity_manager::EntityEvent &e);

    // Methods for the 'calendar' mode
    void next_index(const viridi::entity_manager::EntityEvent &e);
    void previous_index(const viridi::entity_manager::EntityEvent &e);
};

#endif // __ADVENT2022_H__