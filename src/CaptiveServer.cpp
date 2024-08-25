#include "CaptiveServer.h"
#include "CaptiveUI.h"

CaptivePanelServer::CaptivePanelServer() : _server(80), _isConnected(false) {}

void CaptivePanelServer::begin(const char* apName, const char* apPassword) {
    _apName = apName;
    _apPassword = apPassword;
    EEPROM.begin(512); // Initialize EEPROM to store credentials
    loadData();
}

bool CaptivePanelServer::autoConnect() {
    if (_ssid != "" && _password != "") {
        WiFi.begin(_ssid.c_str(), _password.c_str());
        unsigned long startAttemptTime = millis();
        while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) {
            delay(100);
        }
        if (WiFi.status() == WL_CONNECTED) {
            _isConnected = true;
            return true;
        }
    }

    setupAP();
    return false;
}

void CaptivePanelServer::setupAP() {
    WiFi.softAP(_apName, _apPassword);
    _dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());

    _server.on("/", std::bind(&CaptivePanelServer::handleRoot, this));
    _server.on("/save", std::bind(&CaptivePanelServer::handleSave, this));
    _server.onNotFound(std::bind(&CaptivePanelServer::handleNotFound, this));
    _server.begin();

    Serial.println("WiFi Manager started, connect to the AP to configure WiFi");
}

void CaptivePanelServer::handleClient() {
    _dnsServer.processNextRequest();
    _server.handleClient();
}

String CaptivePanelServer::getSSID() {
    return _ssid;
}

String CaptivePanelServer::getPassword() {
    return _password;
}

bool CaptivePanelServer::isConnected() {
    return _isConnected;
}

void CaptivePanelServer::resetSettings() {
    EEPROM.write(0, 0); // Clear the EEPROM
    EEPROM.commit();
    delay(100);
    ESP.restart(); // Restart the ESP to clear credentials
}

void CaptivePanelServer::handleRoot() {
    _server.send(200, "text/html", htmlPage);
}

void CaptivePanelServer::handleSave() {
    String ssid = _server.arg("ssid");
    String password = _server.arg("password");

    if (ssid.length() > 0 && password.length() > 0) {
        saveData(ssid.c_str(), password.c_str());
        _server.send(200, "text/html", "<html><body>Credentials Saved. Restarting...</body></html>");
        delay(2000);
        ESP.restart(); // Restart the ESP to connect with the new credentials
    } else {
        _server.send(200, "text/html", "<html><body>Invalid Input. Please try again.</body></html>");
    }
}

void CaptivePanelServer::handleNotFound() {
    _server.send(404, "text/plain", "Not Found");
}

void CaptivePanelServer::saveData(const char* ssid, const char* password) {
    EEPROM.write(0, 1); // Mark that credentials are saved
    for (int i = 0; i < 32; i++) {
        EEPROM.write(i + 1, ssid[i]);
        EEPROM.write(i + 33, password[i]);
    }
    EEPROM.commit();
}

void CaptivePanelServer::loadData() {
    if (EEPROM.read(0) == 1) {
        char ssid[32], password[32];
        for (int i = 0; i < 32; i++) {
            ssid[i] = EEPROM.read(i + 1);
            password[i] = EEPROM.read(i + 33);
        }
        _ssid = String(ssid);
        _password = String(password);
    } else {
        _ssid = "";
        _password = "";
    }
}
