#include <Arduino.h>
// #include <HardwareSerial.h> // Remove this as it's causing conflicts
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"

// WiFi credentials - IMPORTANT: Replace these with your actual WiFi details before uploading!
const char* ssid = "YOUR_WIFI_SSID";       // Replace with your actual WiFi network name
const char* password = "YOUR_WIFI_PASSWORD"; // Replace with your actual WiFi password

// Define LED pin (built-in LED)
const int ledPin = 2;  // Most ESP32 boards have an LED connected to GPIO2

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Function to print ESP32 chip information
void printESP32Info() {
  esp_chip_info_t chip_info;
  esp_chip_info(&chip_info);
  
  Serial.println("\n\nESP32 Chip Information:");
  Serial.printf("Model: %s\n", ESP.getChipModel());
  Serial.printf("Cores: %d\n", chip_info.cores);
  Serial.printf("Revision: %d\n", chip_info.revision);
  Serial.printf("Flash Size: %d MB\n", spi_flash_get_chip_size() / (1024 * 1024));
  Serial.printf("Free Heap: %d bytes\n", ESP.getFreeHeap());
  Serial.printf("CPU Frequency: %d MHz\n", ESP.getCpuFreqMHz());
  Serial.printf("SDK Version: %s\n", ESP.getSdkVersion());
  Serial.printf("MAC Address: %s\n", WiFi.macAddress().c_str());
  Serial.println("----------------------------------------");
}

// Variables to track uptime
unsigned long startTime;

// Variables for non-blocking LED blink
unsigned long ledLastToggle = 0;
int ledBlinkCount = 0;
int ledBlinkTotal = 0;
bool ledBlinking = false;
bool ledState = false;

void setup() {
  // Initialize built-in LED as output
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);  // Turn off LED initially
  
  // Serial port for debugging purposes
  Serial.begin(115200);
  delay(1000); // Give time for the serial connection to establish
  
  Serial.println("\n\n----- ESP32 Simple Web Server -----");
  Serial.println("ESP32-WROOM-32 Board");
  
  // Print ESP32 chip information
  printESP32Info();
  
  // Initialize SPIFFS
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  startTime = millis();  // Record the start time
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
    // Print ESP32 Local IP Address
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });
  // Route to control the LED
  server.on("/led/on", HTTP_GET, [](AsyncWebServerRequest *request){
    try {
      digitalWrite(ledPin, HIGH);
      request->send(200, "text/plain", "LED is ON");
    } catch (...) {
      request->send(500, "text/plain", "Failed to turn LED on");
    }
  });

  server.on("/led/off", HTTP_GET, [](AsyncWebServerRequest *request){
    try {
      digitalWrite(ledPin, LOW);
      request->send(200, "text/plain", "LED is OFF");
    } catch (...) {
      request->send(500, "text/plain", "Failed to turn LED off");
    }
  });
  // Route for getting uptime
  server.on("/uptime", HTTP_GET, [](AsyncWebServerRequest *request){
    unsigned long currentTime = millis();
    unsigned long elapsedSeconds = (currentTime - startTime) / 1000;
    String uptimeStr = String(elapsedSeconds);
    request->send(200, "text/plain", uptimeStr);
  });

  // Route for system info in JSON format
  server.on("/info", HTTP_GET, [](AsyncWebServerRequest *request){
    String json = "{";
    json += "\"chipModel\":\"" + String(ESP.getChipModel()) + "\",";
    json += "\"chipCores\":" + String(ESP.getChipCores()) + ",";
    json += "\"cpuFreqMHz\":" + String(ESP.getCpuFreqMHz()) + ",";
    json += "\"freeHeap\":" + String(ESP.getFreeHeap()) + ",";
    json += "\"flashSize\":" + String(ESP.getFlashChipSize() / (1024 * 1024)) + ",";
    json += "\"sdkVersion\":\"" + String(ESP.getSdkVersion()) + "\",";
    json += "\"macAddress\":\"" + WiFi.macAddress() + "\",";
    json += "\"ipAddress\":\"" + WiFi.localIP().toString() + "\"";
    json += "}";
    request->send(200, "application/json", json);
  });  // Route for LED blink pattern (example: /blink/3 for 3 blinks)
  server.on("/blink", HTTP_GET, [](AsyncWebServerRequest *request){
    int blinks = 3; // Default number of blinks
    
    if (request->hasParam("count")) {
      blinks = request->getParam("count")->value().toInt();
      if (blinks < 1) blinks = 1;
      if (blinks > 10) blinks = 10; // Limit to 10 blinks max
    }
    
    // Set up non-blocking blink pattern
    ledBlinkTotal = blinks * 2; // Each blink is ON then OFF
    ledBlinkCount = 0;
    ledBlinking = true;
    ledState = true;
    digitalWrite(ledPin, HIGH); // Start with LED ON
    ledLastToggle = millis();
    
    request->send(200, "text/plain", "Started blinking LED " + String(blinks) + " times");
  });
  // Route for scanning WiFi networks
  server.on("/scan-wifi", HTTP_GET, [](AsyncWebServerRequest *request){
    String json = "[";
    int n = WiFi.scanComplete();
    
    if(n == -2) {
      // Scan not triggered yet, start scan
      WiFi.scanNetworks(true); // Start async scan
      request->send(200, "application/json", "[]"); // Return empty array, client should retry
      return;
    } else if(n == -1) {
      // Scan in progress
      request->send(200, "application/json", "[]"); // Return empty array, client should retry
      return;
    } else if(n == 0) {
      // No networks found
      json += "]";
      WiFi.scanDelete();
      if(WiFi.scanComplete() == -2) {
        WiFi.scanNetworks(true); // Start new scan for next request
      }
      request->send(200, "application/json", json);
      return;
    }
    
    // Networks found (n > 0)
    for (int i = 0; i < n; ++i) {
      if(i) json += ",";
      json += "{";
      json += "\"ssid\":\"" + WiFi.SSID(i) + "\",";
      json += "\"rssi\":" + String(WiFi.RSSI(i)) + ",";
      json += "\"encryption\":\"" + String(WiFi.encryptionType(i)) + "\"";
      json += "}";
    }
    json += "]";
    
    // Clean up scan results and start new scan
    WiFi.scanDelete();
    if(WiFi.scanComplete() == -2) {
      WiFi.scanNetworks(true);
    }
    
    request->send(200, "application/json", json);
  });

  // Start server
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  // Check WiFi connection and reconnect if needed
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi disconnected! Attempting to reconnect...");
    WiFi.begin(ssid, password);
  }
  
  // Handle non-blocking LED blinking
  if (ledBlinking) {
    unsigned long currentMillis = millis();
    
    // Toggle the LED state every 200ms
    if (currentMillis - ledLastToggle >= 200) {
      ledLastToggle = currentMillis;
      ledState = !ledState;
      digitalWrite(ledPin, ledState ? HIGH : LOW);
      
      ledBlinkCount++;
      if (ledBlinkCount >= ledBlinkTotal) {
        ledBlinking = false;
        digitalWrite(ledPin, LOW); // Ensure LED is off when done
      }
    }
  }
  
  // Add a small delay to avoid watchdog triggering
  delay(10); // Shorter delay for more responsive blinking
}
