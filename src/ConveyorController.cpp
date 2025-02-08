#include "ConveyorController.h"

ConveyorController::ConveyorController(const char* wifiNetworkName,
   const char* wifiNetworkPassword)
   : wifiNetworkName(wifiNetworkName),
   wifiNetworkPassword(wifiNetworkPassword) {
}

// Public access
void ConveyorController::initIO() {
   // Initialize input pins
   pinMode(PIN_IN_ONOFF, INPUT);
   pinMode(PIN_IN_INCSPEED, INPUT);
   pinMode(PIN_IN_DECSPEED, INPUT);
   pinMode(PIN_IN_LOCALREMOTE, INPUT);

   // Initialize output pins
   pinMode(PIN_OUT_SCL, OUTPUT);
   pinMode(PIN_OUT_SDA, OUTPUT);
   pinMode(PIN_OUT_DECSPEED, OUTPUT);
   pinMode(PIN_OUT_ONOFF, OUTPUT);
   pinMode(PIN_OUT_LOCALREMOTE, OUTPUT);
   pinMode(PIN_OUT_INCSPEED, OUTPUT);

   // Set all output pins to LOW initially
   digitalWrite(PIN_OUT_DECSPEED, LOW);
   digitalWrite(PIN_OUT_ONOFF, LOW);
   digitalWrite(PIN_OUT_LOCALREMOTE, LOW);
   digitalWrite(PIN_OUT_INCSPEED, LOW);
}

void ConveyorController::initLCD() {
   // Initialize and set up the LCD
   lcd.begin(16, 2);
   lcd.init();
   lcd.backlight();
   LCDWaitingForConnection();
}

void ConveyorController::initWeb() {

   // Start serial communication
   Serial.begin(115200);

   // Start WiFi communication with credentials
   WiFi.begin(wifiNetworkName, wifiNetworkPassword);

   // Wait until connected to the network, printing dots
   while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
      LCDWaitingForConnection();
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

   updateLCD();
}

void ConveyorController::assignRoutes() {
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
}

void ConveyorController::startWebServer() {
   // Start the HTTP server
   webServer.begin();
   Serial.println("HTTP server started.");
}

void ConveyorController::handleClient() { webServer.handleClient(); }

void ConveyorController::updateLCD() {
   lcd.clear();
   lcd.print(WiFi.localIP());
   lcd.setCursor(0, 1);
   lcd.print("Speed:");
   lcd.setCursor(9, 1);
   lcd.print(String(conveyorSpeed) + " %");
   conveyorSpeed++;
   delay(200);
}

void ConveyorController::updateState() {
   // Read the state of the input pins
   locRemState = digitalRead(PIN_IN_LOCALREMOTE);
   if (locRemState == HIGH) {
      onOffState = digitalRead(PIN_IN_ONOFF);
      incSpeedState = digitalRead(PIN_IN_INCSPEED);
      decSpeedState = digitalRead(PIN_IN_DECSPEED);
   }
   else {
      onOffState = false;
      incSpeedState = false;
      decSpeedState = false;
   }
}

// Private access
void ConveyorController::mainRoute() {
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

void ConveyorController::unknownRouteResponse() {
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

void ConveyorController::LCDWaitingForConnection() {
   lcd.clear();
   lcd.setCursor(0, 0);
   lcd.print("Waiting for");
   lcd.setCursor(0, 1);
   lcd.print("Hotspot:");
   delay(1000);
   lcd.clear();
   lcd.setCursor(0, 0);
   lcd.print(this->wifiNetworkName);
   lcd.setCursor(0, 1);
   lcd.print(this->wifiNetworkPassword);
   delay(5000);
}


