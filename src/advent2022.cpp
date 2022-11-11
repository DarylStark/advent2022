#include "advent2022.h"

Advent2022::Advent2022()
    : __display("lcd", 0x27, 16, 2),
      __next("next", 4, dsl::arduino_components::input_type::Pullup),
      __previous("previous", 13, dsl::arduino_components::input_type::Pullup),
      __mode("mode", 19, dsl::arduino_components::input_type::Pullup),
      __enter("enter", 18, dsl::arduino_components::input_type::Pullup),
      __leds("leds", 5, 20),
      __ntp_client(__udp)
{
    register_component(__display);
    register_component(__next);
    register_component(__previous);
    register_component(__leds);
    register_component(__mode);
    register_component(__enter);
}

void Advent2022::setup()
{
    // The `_setup` method of the base class will make sure all components
    // are "set-up"
    _setup();

    // Start the time client
    __ntp_client.begin();
    __ntp_client.setTimeOffset(3600);

    // Create entities for the app
    dsl::entity_manager::entities["advent.mode"] = AppMode::Setup;

    // Create entities for the mood lighting
    dsl::entity_manager::entities["advent.mood_current"] = -1;

    // Create entities for the calendar
    dsl::entity_manager::entities["advent.calendar_selected"] = 0;
    dsl::entity_manager::entities["advent.calendar_turn"] = 0;

    // Create the moods
    __moodlightings.reserve(3);
    __moodlightings.push_back({"Off", {}});
    __moodlightings.push_back({"Outline", {0, 1, 18, 19}});
    __moodlightings.push_back({"Full", {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19}});

    // Add listeners to the mode-entities
    dsl::entity_manager::entities["advent.mode"].subscribe(
        "update_mood",
        {std::bind(&Advent2022::configure_mode, this)});
    dsl::entity_manager::entities["advent.mood_current"].subscribe(
        "update_mood_current",
        {std::bind(&Advent2022::set_mood, this)});
    dsl::entity_manager::entities["advent.calendar_selected"].subscribe(
        "update_calendar_index",
        {std::bind(&Advent2022::set_calendar_index, this)});

    // Listener to the mode-button
    dsl::entity_manager::entities["mode.sensor.low"].subscribe(
        "next_mode",
        {std::bind(&Advent2022::next_mode_button, this, std::placeholders::_1)});

    dsl::entity_manager::entities["lcd.display.line0"] = std::string("  Klok syncen");
    dsl::entity_manager::entities["lcd.display.line1"] = std::string("");

    update_ntp(true);

    // Set the startmode 'moodlighting'
    dsl::entity_manager::entities["advent.mood_current"] = 0;
    dsl::entity_manager::entities["advent.mode"] = AppMode::MoodLighting;
}

void Advent2022::loop()
{
    // The `_loop` method of the base class will make sure all components
    // are "looped"
    _loop();

    // Update the time
    update_ntp();

    // Delay :)
    delay(100);
}

bool Advent2022::reconnect_to_wifi()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        WiFi.reconnect();
        unsigned long start = millis();
        unsigned long end = start + (10 * 1000); // 10s timeout
        while (WiFi.status() != WL_CONNECTED && millis() < end)
        {
            delay(100);
        }
    }
    return WiFi.status() == WL_CONNECTED;
}

void Advent2022::disconnect_wifi()
{
    WiFi.disconnect();
}

void Advent2022::update_ntp(bool force /* = false */)
{

    if (millis() - __ntp_last_update >= (60 * 60 * 1000) || force)
    {
        if (reconnect_to_wifi())
        {
            // Update
            __ntp_client.update();

            // Wait 10s to get a valid time
            unsigned long start = millis();
            unsigned long end = start + (10 * 1000); // 10s timeout
            while (__ntp_client.isTimeSet() && millis() < end)
            {
                delay(100);
            }

            // If we have the time, we disconnect. Otherwise we need it for the
            // next iteration
            if (__ntp_client.isTimeSet())
            {
                __ntp_last_update = millis();
                disconnect_wifi();
            }
        }
    }
}

Date Advent2022::get_date()
{
    // Convert the Epoch Time to a `tm` struct. This is a bit of C-code, but
    // i cannot find the correct way to do it in C++.
    long time = __ntp_client.getEpochTime();
    tm *x = localtime(&time);

    // Create the Date object
    Date d;
    d.year = x->tm_year + 1900;
    d.month = x->tm_mon + 1;
    d.day = x->tm_mday;

    // Return the created object
    return d;
}

void Advent2022::next_mode_button(const dsl::entity_manager::EntityEvent &e)
{
    if (e.entity)
    {
        dsl::entity_manager::Entity &mode = dsl::entity_manager::entities["advent.mode"];

        if (static_cast<int>(mode) == AppMode::MoodLighting)
        {
            mode = AppMode::Calendar;
            return;
        }

        if (static_cast<int>(mode) == AppMode::Calendar)
        {
            mode = AppMode::MoodLighting;
            return;
        }
    }
}

void Advent2022::configure_mode()
{
    dsl::entity_manager::Entity &mode = dsl::entity_manager::entities["advent.mode"];

    if (static_cast<int>(mode) == AppMode::MoodLighting)
    {
        // Unsubscribe from the events of the calendar mode
        dsl::entity_manager::entities["next.sensor.low"].unsubscribe_all();
        dsl::entity_manager::entities["previous.sensor.low"].unsubscribe_all();

        dsl::entity_manager::entities["next.sensor.low"].subscribe("button_next", {std::bind(&Advent2022::next_mood, this, std::placeholders::_1)});
        dsl::entity_manager::entities["previous.sensor.low"].subscribe("button_prev", {std::bind(&Advent2022::previous_mood, this, std::placeholders::_1)});

        dsl::entity_manager::entities["lcd.display.line0"] = std::string("Sfeerverlichting");
        dsl::entity_manager::entities["lcd.display.line1"] = std::string("");

        set_mood();

        return;
    }

    if (static_cast<int>(mode) == AppMode::Calendar)
    {
        // Check if it is time yet to start the calendar
        Date now = get_date();
        if (now.year != 2022 || now.month != 12)
        {
            dsl::entity_manager::entities["lcd.display.line0"] = std::string("Het is nog geen");
            dsl::entity_manager::entities["lcd.display.line1"] = std::string("december!");
            delay(5000);
            mode = AppMode::MoodLighting;
            return;
        }

        // Unsubscribe from the events of the calendar mode
        dsl::entity_manager::entities["next.sensor.low"].unsubscribe_all();
        dsl::entity_manager::entities["previous.sensor.low"].unsubscribe_all();

        dsl::entity_manager::entities["next.sensor.low"].subscribe("button_next", {std::bind(&Advent2022::next_index, this, std::placeholders::_1)});
        dsl::entity_manager::entities["previous.sensor.low"].subscribe("button_prev", {std::bind(&Advent2022::previous_index, this, std::placeholders::_1)});

        dsl::entity_manager::entities["lcd.display.line0"] = std::string("Kies  een  vakje");
        dsl::entity_manager::entities["lcd.display.line1"] = std::string("en druk  [ENTER]");

        return;
    }
}

void Advent2022::set_mood()
{
    if (static_cast<int>(dsl::entity_manager::entities["advent.mode"]) == AppMode::MoodLighting)
    {
        dsl::entity_manager::Entity &mood_current = dsl::entity_manager::entities["advent.mood_current"];

        // Sanity checks
        if (static_cast<int>(mood_current) < 0)
        {
            mood_current = static_cast<int>(__moodlightings.size() - 1);
            return;
        }

        if (static_cast<int>(mood_current) >= __moodlightings.size())
        {
            mood_current = 0;
            return;
        }

        // Create the string for on the screen
        const std::string &name = __moodlightings[static_cast<int>(mood_current)].name;
        const uint16_t text_space = 12;
        uint16_t spaces_before = 6 - ((name.length() + 1) / 2);
        uint16_t spaces_after = text_space - name.length() - spaces_before;

        // Display the name on the LCD
        std::stringstream lcd_text;
        lcd_text << "< ";
        lcd_text << std::string(spaces_before, ' ');
        lcd_text << name;
        lcd_text << std::string(spaces_after, ' ');
        lcd_text << " >";
        dsl::entity_manager::entities["lcd.display.line1"] = lcd_text.str();

        // Set the lights to the correct color
        __leds.clear();
        for (const uint16_t &led : __moodlightings[static_cast<int>(mood_current)].leds)
        {
            __leds.set_color(led, 1, {0x00, 0x33, 0x00});
        }
        __leds.show();
    }
}

void Advent2022::next_mood(const dsl::entity_manager::EntityEvent &e)
{
    if (e.entity)
        dsl::entity_manager::entities["advent.mood_current"]++;
}

void Advent2022::previous_mood(const dsl::entity_manager::EntityEvent &e)
{
    if (e.entity)
        dsl::entity_manager::entities["advent.mood_current"]--;
}

void Advent2022::next_index(const dsl::entity_manager::EntityEvent &e)
{
    if (e.entity)
        dsl::entity_manager::entities["advent.calendar_selected"]++;
}

void Advent2022::previous_index(const dsl::entity_manager::EntityEvent &e)
{
    if (e.entity)
        dsl::entity_manager::entities["advent.calendar_selected"]--;
}

void Advent2022::set_calendar_index()
{
    if (static_cast<int>(dsl::entity_manager::entities["advent.mode"]) == AppMode::Calendar)
    {
        // TODO: Add code to navigate between boxes
    }
}