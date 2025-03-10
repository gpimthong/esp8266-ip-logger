# ESP8266 IP Logger to Google Sheets

This project logs the public IP address of an ESP8266 device and sends it to a Google Sheet using a Google Apps Script endpoint.

## Features
- Connects to a WiFi network
- Retrieves the public IP address from multiple endpoints (round-robin selection)
- Sends the IP address as a JSON payload to a Google Apps Script
- Logs the response from the server

## Hardware Requirements
- ESP8266-based board (e.g., NodeMCU, Wemos D1 Mini)
- Stable WiFi connection

## Software Requirements
- Arduino IDE
- ESP8266 board package installed
- Required libraries:
  - `ESP8266WiFi.h`
  - `ESP8266HTTPClient.h`
  - `WiFiClientSecure.h`

## Installation & Usage
1. Clone this repository:
   ```sh
   git clone https://github.com/yourusername/esp8266-ip-logger.git
   cd esp8266-ip-logger
   ```
2. Open the `.ino` file in Arduino IDE.
3. Install required libraries if not already installed.
4. Update the following variables in the code:
   ```cpp
   const char* ssid = "YOUR_WIFI_SSID";
   const char* password = "YOUR_WIFI_PASSWORD";
   const char* googleScriptURL = "YOUR_GOOGLE_SCRIPT_URL";
   ```
5. Upload the code to your ESP8266 board.
6. Open the Serial Monitor (115200 baud) to see connection status and IP logs.

## How It Works
1. The ESP8266 connects to the specified WiFi network.
2. It fetches the public IP from a rotating list of services.
3. The IP is sent as a JSON payload (`{"ip":"<ip>"}`) to the Google Apps Script.
4. The response from the script is displayed in the Serial Monitor.
5. The device repeats this process every 2 minutes.

## Example Output (Serial Monitor)
```
Connecting to WiFi...
Connected to WiFi!
Local IP Address: 192.168.1.100
Fetching IP from: https://api.ipify.org
Public IP: 203.0.113.42
Sending payload: {"ip":"203.0.113.42"}
HTTP Response code: 200
Server response: Success
```

## License
This project is open-source under the MIT License.
