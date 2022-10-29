#ifndef __WIFICONNECT_H__
#define __WIFICONNECT_H__

// STL includes
#include <functional>
#include <sstream>

// Arduino includes
#include <WiFi.h>
#include <WebServer.h>

// Nemo Includes
#include <nemoapp.h>
#include <liquidcrystal.h>

class WiFiConnect : public nemo::NemoApp
{
private:
    // The LCD screen
    nemo::LiquidCrystal __display;

    // Web Server
    WebServer __web_server;

    // Connection methods
    void __wifi_event(WiFiEvent_t event);

    // UI methods
    void __set_screen_text(const nemo::EntityEvent &e);

    // WebServer methods
    void __webserver_mainpage();
    void __webserver_save();

public:
    WiFiConnect();
    void setup() override;
    void loop() override;
};

#endif // __WIFICONNECT_H__