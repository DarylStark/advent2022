// Arduino Includes
#include <Arduino.h>
#include <nemoapp.h>

// Include the apps
#include "advent2022.h"
#include "wificonnect.h"

// Global objects for the app
nemo::NemoApp *advent22;

// Arduino needed methods
void setup()
{
    // Setup Serial
    Serial.begin(115200);

    bool wifi_configured = false;

    if (wifi_configured)
        advent22 = new Advent2022;
    else
        advent22 = new WiFiConnect;

    // TODO:
    // - Check if there are WiFi credentials
    //   - If there aren't:
    //     - Create SSID
    //     - Start webserver
    //     - Display notice on display
    //   - If there are
    //     - Connect
    //     - Retrieve time from NTP
    advent22->setup();
}

void loop()
{
    if (advent22 != nullptr)
        advent22->loop();
}
