#include "wificonnect.h"

// Private methods for WiFiConnect

void WiFiConnect::__wifi_event(WiFiEvent_t event)
{
    if (event == ARDUINO_EVENT_WIFI_AP_STACONNECTED)
    {
        viridi::entity_manager::entities["wificonnect.connected_client"] = 2;
        return;
    }

    if (event == ARDUINO_EVENT_WIFI_AP_STADISCONNECTED)
    {
        viridi::entity_manager::entities["wificonnect.connected_client"] = 1;
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
    viridi::entity_manager::entities["wificonnect.connected_client"] = 0;
    viridi::entity_manager::entities["wificonnect.connected_client"].subscribe("update", {std::bind(&WiFiConnect::__set_screen_text, this, std::placeholders::_1)});

    // Set up the webserver

    // Set up WiFi
    WiFi.mode(WIFI_AP);
    WiFi.softAP("Advent2022");
    WiFi.onEvent(std::bind(&WiFiConnect::__wifi_event, this, std::placeholders::_1));

    // Configure the webserver
    __web_server.on("/", std::bind(&WiFiConnect::__webserver_mainpage, this));
    __web_server.on("/save", HTTP_POST, std::bind(&WiFiConnect::__webserver_save, this));

    // Start the webserver
    __web_server.begin();

    // Set a message on the screen about connecting
    viridi::entity_manager::entities["wificonnect.connected_client"] = 1;
}

void WiFiConnect::loop()
{
    // The `_loop` method of the base class will make sure all components
    // are "looped"
    _loop();

    // Loop the webserver
    __web_server.handleClient();

    // Delay :)
    delay(50);
}

void WiFiConnect::__set_screen_text(const viridi::entity_manager::EntityEvent &e)
{
    if (static_cast<int>(e.entity) == 1)
    {
        viridi::entity_manager::entities["lcd.display.line0"] = std::string("Verbind met WiFi");
        viridi::entity_manager::entities["lcd.display.line1"] = std::string("   Advent2022");
        return;
    }

    if (static_cast<int>(e.entity) == 2)
    {
        viridi::entity_manager::entities["lcd.display.line0"] = std::string(" Navigeer naar");
        viridi::entity_manager::entities["lcd.display.line1"] = std::string("  192.168.4.1");
        return;
    }

    if (static_cast<int>(e.entity) == 3)
    {
        viridi::entity_manager::entities["lcd.display.line0"] = std::string("  Vul je WiFi");
        viridi::entity_manager::entities["lcd.display.line1"] = std::string("  gegevens in");
        return;
    }

    if (static_cast<int>(e.entity) == 4)
    {
        viridi::entity_manager::entities["lcd.display.line0"] = std::string("    Gelukt !");

        for (uint16_t i = 3; i > 0; i--)
        {
            std::stringstream s;
            s << "Herstarten in " << i << "s";
            viridi::entity_manager::entities["lcd.display.line1"] = std::string(s.str());
            delay(1000);
        }

        viridi::entity_manager::entities["lcd.display.line0"] = std::string("  Rebooting...");
        viridi::entity_manager::entities["lcd.display.line1"] = std::string("");

        delay(1000);

        ESP.restart();

        return;
    }
}

void WiFiConnect::__webserver_mainpage()
{
    // Client connected
    viridi::entity_manager::entities["wificonnect.connected_client"] = 3;

    // Create the page to return
    std::stringstream page;
    page << "<html>";
    page << "<head>";
    page << "<title>Advent Kalender 2022</title>";
    page << "<meta name='viewport' content='width=device-width, minimum-scale=1.0, maximum-scale=1.0, user-scalable=no'>";
    page << "<style type='text/css'>";
    page << "body { background-color: #2e5214; color: #000; font-family: arial,sans-serif; font-size: 14px;; }";
    page << "div { box-sizing: border-box; padding: 20px; max-width: 100%; width: 520px; margin: auto; background-color: #fff; text-align: center; }";
    page << "</style>";
    page << "</head>";
    page << "<body>";
    page << "<div>";
    page << "<h1>Advent Kalender 2022</h1>";
    page << "<form action='/save' method='post'>";
    page << "<p><b>SSID</b></p>";
    page << "<p><input type='text' name='ssid' /></p>";
    page << "<p><b>Wachtwoord</b></p>";
    page << "<p><input type='password' name='password' /></p>";
    page << "<p><input type='submit' value='Opslaan' /></p>";
    page << "</form>";
    page << "</div>";
    page << "</body>";
    page << "</html>";

    // Return the page
    __web_server.send(200, "text/html", String(page.str().c_str()));
}

void WiFiConnect::__webserver_save()
{
    // Get the fields. We retrieve these as Arduio-Strings because the method
    // for the `preferences` library that saves the strings expects a Arduino
    // String. If we convert it to a STL string, we have to revert it later on
    // which is not efficient.
    String ssid(__web_server.arg("ssid"));
    String password(__web_server.arg("password"));

    // Save the settings
    __preferences.begin("wifi", false);
    __preferences.putString("ssid", ssid);
    __preferences.putString("password", password);

    // Create the page to return
    std::stringstream page;
    page << "<html>";
    page << "<head>";
    page << "<title>Advent Kalender 2022</title>";
    page << "<meta name='viewport' content='width=device-width, minimum-scale=1.0, maximum-scale=1.0, user-scalable=no'>";
    page << "<style type='text/css'>";
    page << "body { background-color: #2e5214; color: #000; font-family: arial,sans-serif; font-size: 14px;; }";
    page << "div { box-sizing: border-box; padding: 20px; max-width: 100%; width: 520px; margin: auto; background-color: #fff; text-align: center; }";
    page << "</style>";
    page << "</head>";
    page << "<body>";
    page << "<div>";
    page << "<h1>Advent Kalender 2022</h1>";
    page << "<p>Gelukt!</p>";
    page << "</div>";
    page << "</body>";
    page << "</html>";

    // Return the page
    __web_server.send(200, "text/html", String(page.str().c_str()));

    // Reboot!
    viridi::entity_manager::entities["wificonnect.connected_client"] = 4;
}