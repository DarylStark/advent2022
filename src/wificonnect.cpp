#include "wificonnect.h"

#include <functional>

// Private methods for WiFiConnect

void WiFiConnect::__wifi_event(WiFiEvent_t event)
{
    if (event == ARDUINO_EVENT_WIFI_AP_STACONNECTED)
    {
        NemoApp::entities["wificonnect.connected_client"] = 2;
        return;
    }

    if (event == ARDUINO_EVENT_WIFI_AP_STADISCONNECTED)
    {
        NemoApp::entities["wificonnect.connected_client"] = 1;
        return;
    }
}

// Public methods for WiFiConnect

WiFiConnect::WiFiConnect() : __display("lcd", 0x27, 16, 2)
{
    register_component(__display);
}

void WiFiConnect::setup()
{
    // The `_setup` method of the base class will make sure all components
    // are "set-up"
    _setup();

    // Set local entities
    NemoApp::entities["wificonnect.connected_client"] = 0;
    NemoApp::entities["wificonnect.connected_client"].subscribe("update", {std::bind(&WiFiConnect::__set_screen_text, this, std::placeholders::_1)});

    // Set up WiFi
    WiFi.mode(WIFI_AP);
    WiFi.softAP("Advent2022");
    WiFi.onEvent(std::bind(&WiFiConnect::__wifi_event, this, std::placeholders::_1));

    // Set a message on the screen about connecting
    NemoApp::entities["wificonnect.connected_client"] = 1;
}

void WiFiConnect::loop()
{
    // The `_loop` method of the base class will make sure all components
    // are "looped"
    _loop();

    // Delay :)
    delay(50);
}

void WiFiConnect::__set_screen_text(const nemo::EntityEvent &e)
{
    if (static_cast<int>(e.entity) == 1)
    {
        NemoApp::entities["lcd.display.line0"] = std::string("Verbind met WiFi");
        NemoApp::entities["lcd.display.line1"] = std::string("   Advent2022");
        return;
    }

    if (static_cast<int>(e.entity) == 2)
    {
        NemoApp::entities["lcd.display.line0"] = std::string(" Navigeer naar");
        NemoApp::entities["lcd.display.line1"] = std::string("http://10.4.0.1/");
        return;
    }
}