// Arduino Includes
#include <Arduino.h>
#include "advent2022.h"

// Global objects for the app
Advent2022 advent22;

// Arduino needed methods
void setup()
{
    // Setup Serial
    Serial.begin(115200);

    // TODO:
    // - Check if there are WiFi credentials
    //   - If there aren't:
    //     - Create SSID
    //     - Start webserver
    //     - Display notice on display
    //   - If there are
    //     - Connect
    //     - Retrieve time from NTP
    advent22.setup();
}

void loop()
{
    advent22.loop();
}
