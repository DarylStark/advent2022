#include "advent2022.h"

Advent2022::Advent2022() : __display("lcd", 0x27, 16, 2), __next("next", 19, nemo::input_type::Pullup), __previous("previous", 18, nemo::input_type::Pullup)
{
    register_component(__display);
    register_component(__next);
    register_component(__previous);
}

void Advent2022::setup()
{
    // The `_setup` method of the base class will make sure all components
    // are "set-up"
    _setup();

    // Create entities for the app
    NemoApp::entities["advent.mode"] = AppMode::Setup;

    // Create entities for the mood lighting
    NemoApp::entities["advent.mood_current"] = -1;

    // Create entities for the calendar
    NemoApp::entities["advent.calendar_selected"] = 0;
    NemoApp::entities["advent.calendar_turn"] = 0;

    // Create the moods
    __moodlightings.reserve(3);
    __moodlightings.push_back("Off");
    __moodlightings.push_back("Outline");
    __moodlightings.push_back("Full");

    // Add listeners to the mode-entities
    NemoApp::entities["advent.mode"].subscribe(
        "update_mood",
        {std::bind(&Advent2022::configure_mode, this)});
    NemoApp::entities["advent.mood_current"].subscribe(
        "update_mood_current",
        {std::bind(&Advent2022::set_mood, this)});

    // Set the startmode 'moodlighting'
    NemoApp::entities["advent.mode"] = AppMode::MoodLighting;
    NemoApp::entities["advent.mood_current"] = 0;
}

void Advent2022::loop()
{
    // The `_loop` method of the base class will make sure all components
    // are "looped"
    _loop();

    // Delay :)
    delay(50);
}

void Advent2022::configure_mode()
{
    const nemo::Entity &mode = NemoApp::entities["advent.mode"];

    if (static_cast<int>(mode) == AppMode::MoodLighting)
    {
        // Unsubscribe from the events of the calendar mode
        NemoApp::entities["next.sensor.low"].unsubscribe_all();
        NemoApp::entities["previous.sensor.low"].unsubscribe_all();

        NemoApp::entities["next.sensor.low"].subscribe("button_next", {std::bind(&Advent2022::next_mood, this, std::placeholders::_1)});
        NemoApp::entities["previous.sensor.low"].subscribe("button_prev", {std::bind(&Advent2022::previous_mood, this, std::placeholders::_1)});

        NemoApp::entities["lcd.display.line0"] = std::string("Sfeerverlichting");
        NemoApp::entities["lcd.display.line1"] = std::string("");
        return;
    }

    if (static_cast<int>(mode) == AppMode::Calendar)
    {
        // Unsubscribe from the events of the calendar mode
        NemoApp::entities["next.sensor.low"].unsubscribe_all();
        NemoApp::entities["previous.sensor.low"].unsubscribe_all();

        NemoApp::entities["next.sensor.low"].subscribe("button_next", {std::bind(&Advent2022::next_index, this, std::placeholders::_1)});
        NemoApp::entities["previous.sensor.low"].subscribe("button_prev", {std::bind(&Advent2022::previous_index, this, std::placeholders::_1)});

        NemoApp::entities["lcd.display.line0"] = std::string("Kies  een  vakje");
        NemoApp::entities["lcd.display.line1"] = std::string("en druk  [ENTER]");

        return;
    }
}

void Advent2022::set_mood()
{
    if (static_cast<int>(NemoApp::entities["advent.mode"]) == AppMode::MoodLighting)
    {
        nemo::Entity &mood_current = NemoApp::entities["advent.mood_current"];

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
        const std::string &name = __moodlightings[static_cast<int>(mood_current)];
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
        NemoApp::entities["lcd.display.line1"] = lcd_text.str();

        // TODO:
        // Set the lights to the correct color
    }
}

void Advent2022::next_mood(const nemo::EntityEvent &e)
{
    if (e.entity)
        NemoApp::entities["advent.mood_current"]++;
}

void Advent2022::previous_mood(const nemo::EntityEvent &e)
{
    if (e.entity)
        NemoApp::entities["advent.mood_current"]--;
}

void Advent2022::next_index(const nemo::EntityEvent &e)
{
    if (e.entity)
        NemoApp::entities["advent.calendar_turn"]++;
}

void Advent2022::previous_index(const nemo::EntityEvent &e)
{
    if (e.entity)
        NemoApp::entities["advent.calendar_turn"]--;
}
