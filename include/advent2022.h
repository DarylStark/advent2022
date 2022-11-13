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
#include <arduino_apps/appbase.h>
#include <arduino_components/liquidcrystal.h>
#include <arduino_components/binarysensor.h>
#include <arduino_components/ws2812b.h>

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

class Advent2022 : public dsl::arduino_apps::AppBase
{
private:
    // The LCD screen
    dsl::arduino_components::LiquidCrystal __display;

    // Objects for NTP
    WiFiUDP __udp;
    NTPClient __ntp_client;
    unsigned long __ntp_last_update;

    // The buttons
    dsl::arduino_components::BinarySensor __next;
    dsl::arduino_components::BinarySensor __previous;
    dsl::arduino_components::BinarySensor __mode;
    dsl::arduino_components::BinarySensor __enter;

    // LED strip
    dsl::arduino_components::WS2812B __leds;

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
    void next_mode_button(const dsl::entity_manager::EntityEvent &e);
    void configure_mode();

    // Methods for the 'moodlighting' mode
    void set_mood();
    void next_mood(const dsl::entity_manager::EntityEvent &e);
    void previous_mood(const dsl::entity_manager::EntityEvent &e);
    void toggle_lcd(const dsl::entity_manager::EntityEvent &e);

    // Methods for the 'calendar' mode
    void next_index(const dsl::entity_manager::EntityEvent &e);
    void previous_index(const dsl::entity_manager::EntityEvent &e);
    void set_calendar_index();
};

#endif // __ADVENT2022_H__