#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>

// WiFi credentials
const char* ssid = "x";
const char* password = "x";

// Google Apps Script URL for logging data
const char* googleScriptURL = "https://script.google.com/macros/s/x/exec";

// Secure WiFi client
WiFiClientSecure client;

// List of public IP address providers for redundancy
const char* endpoints[] = {
    "https://api.ipify.org",
    "https://ifconfig.me/ip",
    "https://checkip.amazonaws.com",
    "https://ipinfo.io/ip",
    "https://icanhazip.com"
};
const int numEndpoints = sizeof(endpoints) / sizeof(endpoints[0]);
int currentEndpointIndex = 0; // Index to track which endpoint is used next

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);

    Serial.print("Connecting to WiFi...");
    // Wait until connected to WiFi
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }

    Serial.println("\nConnected to WiFi!");
    Serial.print("Local IP Address: ");
    Serial.println(WiFi.localIP());

    client.setInsecure();  // Ignore SSL certificate validation
}

// Function to get the public IP address from a rotating set of endpoints
String getPublicIP() {
    HTTPClient http;
    String ip = "Unknown";
    const char* endpoint = endpoints[currentEndpointIndex];
    
    Serial.print("Fetching IP from: ");
    Serial.println(endpoint);

    http.begin(client, endpoint);
    int httpResponseCode = http.GET();
    
    if (httpResponseCode > 0) {
        ip = http.getString();
        ip.trim(); // Remove extra spaces or newline characters
        Serial.print("Public IP: ");
        Serial.println(ip);
    } else {
        Serial.print("Failed to get IP. HTTP error code: ");
        Serial.println(httpResponseCode);
    }
    
    http.end();
    
    // Rotate to the next endpoint in a round-robin manner
    currentEndpointIndex = (currentEndpointIndex + 1) % numEndpoints;
    return ip;
}

void loop() {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        String ip = getPublicIP();  // Get the current public IP address

        // Create JSON payload including the IP address
        String payload = "{\"ip\":\"" + ip + "\"}";
        
        Serial.print("Sending payload: ");
        Serial.println(payload);

        // Initialize HTTP request to Google Apps Script
        http.begin(client, googleScriptURL);
        http.addHeader("Content-Type", "application/json");
        http.setTimeout(15000);  // Set timeout to 15 seconds

        // Send the POST request with the payload
        int httpResponseCode = http.POST(payload);
    
        // Handle response
        if (httpResponseCode > 0) {
            Serial.print("HTTP Response code: ");
            Serial.println(httpResponseCode);
            String response = http.getString();
            Serial.println("Server response: " + response);
        } else {
            Serial.print("Error sending data. HTTP error code: ");
            Serial.println(httpResponseCode);
        }
    
        http.end();
    } else {
        Serial.println("WiFi not connected. Retrying...");
    }

    // Wait for 2 minutes before the next data transmission
    delay(120000);
}
