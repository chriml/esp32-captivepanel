#include <CaptiveServer.h>

CaptivePanelServer CaptivePanelServer;

void setup() {
    Serial.begin(115200);
    
    // Initialize WiFi Manager
    CaptivePanelServer.begin("ESP32_AP", "password123");

    // Attempt to connect automatically
    if (!CaptivePanelServer.autoConnect()) {
        Serial.println("Failed to connect automatically. Please configure WiFi through the AP.");
    } else {
        Serial.println("Connected to WiFi!");
    }
}

void loop() {
    // Handle clients (required when in AP mode)
    CaptivePanelServer.handleClient();

    if (CaptivePanelServer.isConnected()) {
        // Your main code here
    }
}
