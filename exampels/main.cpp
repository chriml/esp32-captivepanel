#include "CaptivePortal.h"

// Define the input fields for the form
InputField fields[] = {
    {"Device Name", "device_name", "text", "ESP32", ""},
    {"WiFi SSID", "wifi_ssid", "text", "", ""},
    {"WiFi Password", "wifi_password", "text", "", ""},
    {"Mode", "mode", "select", "station", "station,access_point"}};

// calculate length
const int fieldCount = sizeof(fields) / sizeof(fields[0]);

CaptivePortal portal("ESP32-Captive-Portal", "12345678", fields, fieldCount);

void setup()
{
    Serial.begin(115200);
    while (!Serial)
    {
        delay(10);
    }
    delay(1000);

    // start portal
    portal.begin();
    Serial.println("ESP32 Captive Sportal available");
}

void loop()
{
    // loop the portal (checks for dns requests)
    portal.loop();
    delay(10);

    Serial.println(portal.getField('wifi_ssid'));
    portal.setField('wifi_ssid', "234");
}
