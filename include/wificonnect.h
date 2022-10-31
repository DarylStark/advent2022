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
#include <appbase.h>
#include <liquidcrystal.h>

class WiFiConnect : public viridi::apps::AppBase
{
private:
    // The LCD screen
    viridi::components::LiquidCrystal __display;

    // Saving of settings
    Preferences __preferences;

    // Web Server
    WebServer __web_server;

    // Connection methods
    void __wifi_event(WiFiEvent_t event);

    // UI methods
    void __set_screen_text(const viridi::entity_manager::EntityEvent &e);

    // WebServer methods
    void __webserver_mainpage();
    void __webserver_save();

public:
    WiFiConnect();
    void setup() override;
    void loop() override;
};

#endif // __WIFICONNECT_H__