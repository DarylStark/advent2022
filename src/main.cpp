// STL includes
#include <string>

// Arduino Includes
#include <Arduino.h>
#include <apps/appbase.h>

// Include the apps
#include "advent2022.h"
#include "wificonnect.h"

#include <components/liquidcrystal.h>

// Global objects for the app
viridi::apps::AppBase *advent22;

// Arduino needed methods
void setup()
{
    // Setup Serial
    Serial.begin(115200);

    // Flag if we are connected to WiFi
    bool wifi_configured = false;

    // Try to connect to Wifi (if it is configured). We do this in a scoped
    // block to make sure all variabled are destroyed after this and won't
    // interfere with the application itself.
    {
        viridi::components::LiquidCrystal display("lcd", 0x27, 16, 2);
        display.setup();

        Preferences preferences;
        preferences.begin("wifi", false);
        String ssid = preferences.getString("ssid");
        String password = preferences.getString("password");

        viridi::apps::AppBase::entities["lcd.display.line0"] = std::string("Verbinden met");
        viridi::apps::AppBase::entities["lcd.display.line1"] = std::string(ssid.c_str());

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
                viridi::apps::AppBase::entities["lcd.display.line0"] = std::string("   Verbinden");
                viridi::apps::AppBase::entities["lcd.display.line1"] = std::string("    mislukt");
                delay(3000);
            }
            else
            {
                viridi::apps::AppBase::entities["lcd.display.line0"] = std::string("   Verbonden!");
                viridi::apps::AppBase::entities["lcd.display.line1"] = std::string("");
                delay(3000);
            }
        }

        // TODO: This should be done automatically
        viridi::apps::AppBase::entities["lcd.display.backlight"].unsubscribe_all();
        viridi::apps::AppBase::entities["lcd.display.line0"].unsubscribe_all();
        viridi::apps::AppBase::entities["lcd.display.line1"].unsubscribe_all();
    }

    if (wifi_configured)
        advent22 = new Advent2022;
    else
        advent22 = new WiFiConnect;

    advent22->setup();
}

void loop()
{
    if (advent22 != nullptr)
        advent22->loop();
}
