#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <WiFiClient.h>
#include <ESPmDNS.h>
#include <EEPROM.h>


class CaptivePanelServer {
public:
    CaptivePanelServer();
    void begin(const char* apName, const char* apPassword = nullptr);
    bool autoConnect();
    void handleClient();
    String getSSID();
    String getPassword();
    bool isConnected();
    void resetSettings();

private:
    void setupAP();
    void handleRoot();
    void handleSave();
    void handleNotFound();
    void saveData(const char* ssid, const char* password);
    void loadData();
    
    const char* _apName;
    const char* _apPassword;
    String _ssid;
    String _password;
    WebServer _server;
    DNSServer _dnsServer;
    const byte DNS_PORT = 53;
    bool _isConnected;
};