#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <LiquidCrystal_I2C.h>
#include <WiFiClient.h>
#include <Wire.h>
#include "globalDefinitions.h"
#include "pinDefinitions.h"

class ConveyorController {
  public:
   ConveyorController(const char* wifiNetworkName,
                      const char* wifiNetworkPassword)
       : wifiNetworkName(wifiNetworkName),
         wifiNetworkPassword(wifiNetworkPassword) {}

   void begin() {
      // Initialize input pins
      pinMode(PIN_IN_ONOFF, INPUT);
      pinMode(PIN_IN_INCSPEED, INPUT);
      pinMode(PIN_IN_SCL, INPUT);
      pinMode(PIN_IN_SDA, INPUT);
      pinMode(PIN_IN_DECSPEED, INPUT);
      pinMode(PIN_IN_LOCALREMOTE, INPUT);

      // Initialize output pins
      pinMode(PIN_OUT_DECSPEED, OUTPUT);
      pinMode(PIN_OUT_ONOFF, OUTPUT);
      pinMode(PIN_OUT_LOCALREMOTE, OUTPUT);
      pinMode(PIN_OUT_INCSPEED, OUTPUT);

      // Set all output pins to LOW initially
      digitalWrite(PIN_OUT_DECSPEED, LOW);
      digitalWrite(PIN_OUT_ONOFF, LOW);
      digitalWrite(PIN_OUT_LOCALREMOTE, LOW);
      digitalWrite(PIN_OUT_INCSPEED, LOW);

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
      Serial.println(this->wifiNetworkName);
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());

      // Check MDNS functionality
      if (MDNS.begin("espwebserver")) {
         Serial.println("MDNS responder started.");
      }

      // Assign routes
      webServer.on("/", [this]() { mainRoute(); });
      webServer.on("/example", [this]() {
         String response = "Sample route for multiple pages.";
         webServer.send(200, "text/plain", response);
      });
      webServer.on("/ledON", [this]() {
         Serial.println("Turning LED ON.");
         mainRoute();
      });
      webServer.on("/ledOFF", [this]() {
         Serial.println("Turning LED OFF.");
         mainRoute();
      });
      webServer.onNotFound([this]() { unknownRouteResponse(); });

      // Start the HTTP server
      webServer.begin();
      Serial.println("HTTP server started.");

      // Initialize and set up the LCD
      lcd.init();
      lcd.backlight();
      lcd.print("Robojax D1 Mini");
   }

   void handleClient() { webServer.handleClient(); }

   void updateLCD() {
      lcd.clear();
      lcd.print(WiFi.localIP());
      lcd.setCursor(0, 1);
      lcd.print("Counting:");
      lcd.setCursor(9, 1);
      lcd.print(counter);
      counter++;
      delay(200);
   }

  private:
   // WiFi credentials
   const char* wifiNetworkName;
   const char* wifiNetworkPassword;

   // Web server
   ESP8266WebServer webServer = ESP8266WebServer(80);

   // LCD
   LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);

   // Counter
   int counter = 0;

   // Route handler for the main page
   void mainRoute() {
      String secondsSinceStart = String(millis() / 1000);
      String response = "Hello Arduino World!<br>";
      response += "<br>Time since Arduino started: ";
      response += secondsSinceStart;
      response += " seconds.<br><br>";
      response += "LED status: ";
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
         response +=
             " " + webServer.argName(i) + ": " + webServer.arg(i) + "\n";
      }
      webServer.send(404, "text/plain", response);
   }
};

// Global instance of the controller
ConveyorController* conveyorController;

void setup() {
   conveyorController =
       new ConveyorController(wifiNetworkName, wifiNetworkPassword);
   conveyorController->begin();
}

void loop() {
   conveyorController->handleClient();
   conveyorController->updateLCD();
   delay(10);
}