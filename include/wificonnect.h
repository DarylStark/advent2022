#ifndef __WIFICONNECT_H__
#define __WIFICONNECT_H__

#include <WiFi.h>

// Nemo Includes
#include <nemoapp.h>
#include <liquidcrystal.h>

class WiFiConnect : public nemo::NemoApp
{
private:
    // The LCD screen
    nemo::LiquidCrystal __display;

    // Connection methods
    void __wifi_event(WiFiEvent_t event);

    // UI methods
    void __set_screen_text(const nemo::EntityEvent &e);

public:
    WiFiConnect();
    void setup() override;
    void loop() override;
};

#endif // __WIFICONNECT_H__