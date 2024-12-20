#ifndef __WIFICONNECT_H__
#define __WIFICONNECT_H__

// STL includes
#include <functional>
#include <sstream>

// Arduino includes
#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>

// Viridi Includes
#include <arduino_apps/appbase.h>
#include <arduino_components/liquidcrystal.h>

class WiFiConnect : public dsl::arduino_apps::AppBase
{
private:
    // The LCD screen
    dsl::arduino_components::LiquidCrystal __display;

    // Saving of settings
    Preferences __preferences;

    // Web Server
    WebServer __web_server;

    // Connection methods
    void __wifi_event(WiFiEvent_t event);

    // UI methods
    void __set_screen_text(const dsl::entity_manager::EntityEvent &e);

    // WebServer methods
    void __webserver_mainpage();
    void __webserver_save();

public:
    WiFiConnect();
    void setup() override;
    void loop() override;
};

#endif // __WIFICONNECT_H__