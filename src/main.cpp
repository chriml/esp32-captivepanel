#include <WiFi.h>
#include "CaptivePortal.h"

// Define the input fields for the form
InputField fields[] = {
    {"Device Name", "device_name", "text", "ESP32", ""},
    {"WiFi SSID", "wifi_ssid", "text", "", ""},
    {"WiFi Password", "wifi_password", "text", "", ""},
    {"Mode", "mode", "select", "station", "station,access_point"}};

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

    portal.begin();
    Serial.println("ESP32 Captive Sportal avalable");
}

void loop()
{
    // check for dns server requests
    portal.loop();
    delay(10);
}
