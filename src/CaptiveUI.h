// HTML content as a string imported from WiFiManagerUI.h
const char* htmlPage = R"====(
<!DOCTYPE html>
<html>
<head>
    <title>ESP32 Wi-Fi Setup</title>
    <style>
        body { font-family: Arial, sans-serif; margin: 50px; }
        h1 { color: #333; }
        input[type="text"], input[type="password"] { width: 100%; padding: 10px; margin: 10px 0; }
        input[type="submit"] { padding: 10px 20px; background-color: #007BFF; color: white; border: none; cursor: pointer; }
    </style>
</head>
<body>
    <h1>ESP32 Wi-Fi Setup</h1>
    <form action="/submit" method="POST">
        <label for="ssid">SSID:</label>
        <input type="text" id="ssid" name="ssid" required>
        <label for="password">Password:</label>
        <input type="password" id="password" name="password" required>
        <input type="submit" value="Connect">
    </form>
</body>
</html>
)====";