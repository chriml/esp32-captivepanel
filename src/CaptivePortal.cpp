#include "CaptivePortal.h"
#include "Preferences.h"

Preferences preferences;

CaptivePortal::CaptivePortal(const char *ssid, const char *password, InputField *fields, int fieldCount)
    : _ssid(ssid), _password(password), _fields(fields), _fieldCount(fieldCount), _server(80)
{
}

void CaptivePortal::begin()
{
    startAccessPoint();

    _server.on("/", HTTP_GET, std::bind(&CaptivePortal::handleRootRequest, this, std::placeholders::_1));
    _server.on("/submit", HTTP_POST, std::bind(&CaptivePortal::handleSubmitRequest, this, std::placeholders::_1));
    _server.onNotFound(std::bind(&CaptivePortal::handleNotFound, this, std::placeholders::_1));

    _server.begin();
}

void CaptivePortal::loop()
{
    _dnsServer.processNextRequest();
}

void CaptivePortal::startAccessPoint()
{
    WiFi.softAP(_ssid, _password);
    WiFi.onEvent([this](WiFiEvent_t event, WiFiEventInfo_t info)
                 {        
        if (event == 14) { // Client connects
            IPAddress ip = WiFi.softAPIP();
            String macAddress = WiFi.BSSIDstr();
        } });
    _dnsServer.start(53, "*", WiFi.softAPIP());
}

String CaptivePortal::generateFormHTML()
{
    String html = "<form action='/submit' method='post'>";

    for (int i = 0; i < _fieldCount; i++)
    {
        html += "<label for='" + _fields[i].name + "'>" + _fields[i].label + ":</label><br>";

        if (_fields[i].type == "text")
        {
            String value = preferences.getString(_fields[i].name.c_str());
            value = value.isEmpty() ? _fields[i].value : value;
            html += "<input type='text' id='" + _fields[i].name + "' name='" + _fields[i].name + "' value='" + _fields[i].value + "'><br><br>";
        }
        else if (_fields[i].type == "select")
        {
            html += "<select id='" + _fields[i].name + "' name='" + _fields[i].name + "'>";

            int start = 0;
            int end = _fields[i].options.indexOf(',');
            while (end != -1)
            {
                String option = _fields[i].options.substring(start, end);
                html += "<option value='" + option + "'";
                if (option == _fields[i].value)
                {
                    html += " selected";
                }
                html += ">" + option + "</option>";
                start = end + 1;
                end = _fields[i].options.indexOf(',', start);
            }

            // Last or only option
            String option = _fields[i].options.substring(start);
            html += "<option value='" + option + "'";
            if (option == _fields[i].value)
            {
                html += " selected";
            }
            html += ">" + option + "</option>";

            html += "</select><br><br>";
        }
    }

    html += "<input type='submit' value='Submit'>";
    html += "</form>";
    return html;
}

void CaptivePortal::handleRootRequest(AsyncWebServerRequest *request)
{
    String html = "<html><body>";
    html += "<h1>Configure your device</h1>";
    html += generateFormHTML();
    html += "</body></html>";

    request->send(200, "text/html", html);
}

void CaptivePortal::handleNotFound(AsyncWebServerRequest *request)
{
    String html = "<html><body>";
    html += "<h1>Configure your device</h1>";
    html += generateFormHTML();

    html += "</body></html>";

    request->send(200, "text/html", html);
}

void CaptivePortal::handleSubmitRequest(AsyncWebServerRequest *request)
{
    String response = "<html><body>";
    response += "<h1>Submitted Values</h1><ul>";

    for (int i = 0; i < _fieldCount; i++)
    {
        if (request->hasParam(_fields[i].name, true))
        {
            preferences.putString(_fields[i].name.c_str(), request->getParam(_fields[i].name, true)->value());

            String value = request->getParam(_fields[i].name, true)->value();
            _fields[i].value = value;
            response += "<li>" + _fields[i].label + ": " + value + "</li>";
        }
    }

    response += "</ul>";
    response += "<a href='/'>Go Back</a>";
    response += "</body></html>";

    request->send(200, "text/html", response);
}


String CaptivePortal::getField(char *field)
{
    return preferences.getString(field);
}

void CaptivePortal::setField(char *field, String value)
{
    preferences.putString(field, value);
}

