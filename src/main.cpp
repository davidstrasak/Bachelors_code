#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <LiquidCrystal_I2C.h>
#include <WiFiClient.h>
#include <Wire.h>

// Global variables for WiFi credentials
const char* wifiNetworkName = "TP-Link_83CA";
const char* wifiNetworkPassword = "65362280";

// Initialize the web server at port 80
ESP8266WebServer webServer(80);

// Relay control pin
#define relayControlPin D6

// Create an LCD instance with address 0x27 for a 16x2 display
LiquidCrystal_I2C lcd(0x27, 16, 2);

int counter = 0;

// Route handler for the main page
void mainRoute() {
   String secondsSinceStart = String(millis() / 1000);
   String response = "Hello Arduino World!<br>";
   response += "<br>Time since Arduino started: ";
   response += secondsSinceStart;
   response += " seconds.<br><br>";
   response += "LED status: ";
   response += digitalRead(relayControlPin);
   response += "<br><br>";
   response += "<a href=\"/ledON\">Turn LED ON</a><br><br>";
   response += "<a href=\"/ledOFF\">Turn LED OFF</a>";

   webServer.send(200, "text/html", response);
}

// Route handler for unknown pages
void unknownRouteResponse() {
   String response = "Non-existing route\n\n";
   response += "URI: ";
   response += webServer.uri();
   response += "\nMethod: ";
   response += (webServer.method() == HTTP_GET) ? "GET" : "POST";
   response += "\nArguments: ";
   response += webServer.args();
   response += "\n";
   for (uint8_t i = 0; i < webServer.args(); i++) {
      response += " " + webServer.argName(i) + ": " + webServer.arg(i) + "\n";
   }
   webServer.send(404, "text/plain", response);
}

void setup() {
   // Configure relay pin as output and set it LOW
   pinMode(relayControlPin, OUTPUT);
   digitalWrite(relayControlPin, LOW);

   // Start serial communication
   Serial.begin(115200);

   // Start WiFi communication with credentials
   WiFi.begin(wifiNetworkName, wifiNetworkPassword);

   // Wait until connected to the network, printing dots
   while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
   }
   Serial.println("");
   Serial.print("Connected to WiFi network ");
   Serial.println(wifiNetworkName);
   Serial.print("IP address: ");
   Serial.println(WiFi.localIP());

   // Check MDNS functionality
   if (MDNS.begin("espwebserver")) {
      Serial.println("MDNS responder started.");
   }

   // Assign routes
   webServer.on("/", mainRoute);

   webServer.on("/example", []() {
      String response = "Sample route for multiple pages.";
      webServer.send(200, "text/plain", response);
   });

   webServer.on("/ledON", []() {
      digitalWrite(relayControlPin, HIGH);
      Serial.println("Turning LED ON.");
      mainRoute();
   });

   webServer.on("/ledOFF", []() {
      digitalWrite(relayControlPin, LOW);
      Serial.println("Turning LED OFF.");
      mainRoute();
   });

   webServer.onNotFound(unknownRouteResponse);

   // Start the HTTP server
   webServer.begin();
   Serial.println("HTTP server started.");

   // Initialize and set up the LCD
   lcd.init();
   lcd.backlight();
   lcd.print("Robojax D1 Mini");
}

void loop() {
   // Handle incoming web requests
   webServer.handleClient();
   delay(10);

   // Update the LCD display
   lcd.clear();
   lcd.print(WiFi.localIP());
   lcd.setCursor(0, 1);
   lcd.print("Counting:");
   lcd.setCursor(9, 1);
   lcd.print(counter);
   counter++;
   delay(200);
}