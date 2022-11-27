// STL includes
#include <string>

// Arduino Includes
#include <Arduino.h>
#include <arduino_apps/appbase.h>

// Include the apps
#include "advent2022.h"
#include "wificonnect.h"

#include <arduino_components/liquidcrystal.h>

// #define RESET_WIFI 1

// Global objects for the app
dsl::arduino_apps::AppBase *advent22;

// Arduino needed methods
void setup()
{
    // Setup Serial
    Serial.begin(115200);

#ifdef RESET_WIFI
    // Block to delete WIFI details
    Preferences preferences;
    preferences.begin("wifi", false);
    preferences.remove("ssid");
    preferences.remove("password");
    return;
#else

    // Flag if we are connected to WiFi
    bool wifi_configured = false;

    // Try to connect to Wifi (if it is configured). We do this in a scoped
    // block to make sure all variabled are destroyed after this and won't
    // interfere with the application itself.
    {
        dsl::arduino_components::LiquidCrystal display("lcd", 0x27, 16, 2);
        display.setup();

        Preferences preferences;
        preferences.begin("wifi", false);
        String ssid = preferences.getString("ssid");
        String password = preferences.getString("password");

        dsl::entity_manager::entities["lcd.display.line0"] = std::string("Verbinden met");
        dsl::entity_manager::entities["lcd.display.line1"] = std::string(ssid.c_str());

        if (ssid != "")
        {
            WiFi.mode(WIFI_STA);
            WiFi.disconnect();
            WiFi.begin(ssid.c_str(), password.c_str());
            unsigned long start = millis();
            unsigned long end = start + (30 * 1000); // 30s timeout
            while (!WiFi.isConnected() && millis() < end)
            {
                delay(100);
            }
            wifi_configured = WiFi.isConnected();
            if (!wifi_configured)
            {
                dsl::entity_manager::entities["lcd.display.line0"] = std::string("   Verbinden");
                dsl::entity_manager::entities["lcd.display.line1"] = std::string("    mislukt");
                delay(3000);
            }
            else
            {
                dsl::entity_manager::entities["lcd.display.line0"] = std::string("   Verbonden!");
                dsl::entity_manager::entities["lcd.display.line1"] = std::string("");
                delay(1000);
            }
        }
    }

    if (wifi_configured)
        advent22 = new Advent2022;
    else
        advent22 = new WiFiConnect;

    advent22->setup();
#endif
}

void loop()
{
#ifdef RESET_WIFI
    Serial.println("GEEN FIRMWARE");
    delay(1000);
#else
    if (advent22 != nullptr)
        advent22->loop();
#endif
}
