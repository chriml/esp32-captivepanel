#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>

struct InputField
{
    String label;
    String name;
    String type;    // "text", "select", etc.
    String value;   // Default value
    String options; // For select type (comma-separated)
};

class CaptivePortal
{
public:
    CaptivePortal(const char *ssid, const char *password, InputField *fields, int fieldCount);
    void begin();
    void loop();
    String getField(char *field);
    void setField(char *field, String value);

private:
    const char *_ssid;
    const char *_password;
    InputField *_fields;
    int _fieldCount;
    AsyncWebServer _server;
    DNSServer _dnsServer;

    void startAccessPoint();
    String generateFormHTML();
    void handleRootRequest(AsyncWebServerRequest *request);
    void handleSubmitRequest(AsyncWebServerRequest *request);
    void handleNotFound(AsyncWebServerRequest *request);
};
