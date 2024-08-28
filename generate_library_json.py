import configparser
import json
import os


library_data = {
    "name": "ESP32_WifiManager",
    "version": "1.0.0",
    "description": "Generic captive portal ",
    "keywords": "wifi,captive,wifimanager,webserver",
    "homepage": "https://github.com/chriml/CaptivePortal.git",
    "repository": {
        "type": "git",
        "url": "https://github.com/chriml/CaptivePortal.git"
    },
    "authors": [
        {
            "name": "Christian Meinl",
            "maintainer": True
        }
    ],
    "license": "MIT",
    "frameworks": "arduino",
    "platforms": [
        "espressif32",
        "espressif8266",
        "raspberrypi"
    ],
    "dependencies": [
        """ {
            "owner": "mathieucarbou",
            "name": "AsyncTCP",
            "version": "^3.2.4",
            "platforms": "espressif32"
        },
        {
            "owner": "esphome",
            "name": "ESPAsyncTCP-esphome",
            "version": "^2.0.0",
            "platforms": "espressif8266"
        },
        {
            "name": "Hash",
            "platforms": "espressif8266"
        },
        {
            "owner": "khoih-prog",
            "name": "AsyncTCP_RP2040W",
            "version": "^1.2.0",
            "platforms": "raspberrypi"
        } """
    ],
    "export": {
        "include": [
            "examples",
            "src",
            "library.json",
            "library.properties",
            "LICENSE",
            "README.md"
        ]
    },
    "build": {
        "libCompatMode": "strict"
    }
}


def parse_platformio_ini(file_path):
    config = configparser.ConfigParser()
    config.read(file_path)

    if 'platformio' in config:
        library_data['name'] = config['platformio'].get(
            'name', library_data['name'])

    if 'env:lilygo-t-display-s3' in config:
        env_config = config['env:lilygo-t-display-s3']

        # Update homepage and repository URL if available
        # You can customize this
        library_data['homepage'] = "https://github.com/chriml/esp32-captivepanel"
        # Customize this
        library_data['repository']['url'] = "https://github.com/chriml/esp32-captivepanel.git"

        # Update dependencies
        if 'lib_deps' in env_config:
            dependencies = env_config['lib_deps'].split('\n')
            for dep in dependencies:
                dep = dep.strip()
                if dep:
                    library_data['dependencies'].append({
                        "name": dep,
                        "version": "*",  # Use * or specify a version if needed
                        "platforms": "espressif32"  # Adjust if needed
                    })

    return library_data


def generate_library_json():
    ini_path = 'platformio.ini'
    json_path = 'library.json'

    if not os.path.isfile(ini_path):
        print(f"Error: {ini_path} not found!")
        return

    library_data = parse_platformio_ini(ini_path)

    with open(json_path, 'w') as f:
        json.dump(library_data, f, indent=4)
        print(f"library.json generated at {json_path}")


if __name__ == "__main__":
    generate_library_json()
