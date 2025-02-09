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
   webServer.on("/incSpeedOn", [this]() {
      Serial.println("Increasing speed button pressed");
      if (!localRemoteState) {
         digitalWrite(PIN_OUT_INCSPEED, HIGH);
      };
      mainRoute();
      });
   webServer.on("/incSpeedOff", [this]() {
      Serial.println("Increasing speed button released");
      digitalWrite(PIN_OUT_INCSPEED, LOW);
      mainRoute();
      });
   webServer.on("/decSpeedOn", [this]() {
      Serial.println("Decreasing speed button pressed");
      if (!localRemoteState) {
         digitalWrite(PIN_OUT_DECSPEED, HIGH);
      };
      mainRoute();
      });
   webServer.on("/decSpeedOff", [this]() {
      Serial.println("Decreasing speed button released");
      digitalWrite(PIN_OUT_DECSPEED, LOW);
      mainRoute();
      });
   webServer.on("/conveyorOn", [this]() {
      Serial.println("Conveyor turned ON");
      if (!localRemoteState) {
         digitalWrite(PIN_OUT_ONOFF, HIGH);
      };
      mainRoute();
      });
   webServer.on("/conveyorOff", [this]() {
      Serial.println("Conveyor turned OFF");
      digitalWrite(PIN_OUT_ONOFF, LOW);
      mainRoute();
      });
   webServer.onNotFound([this]() { unknownRouteResponse(); });
}

void ConveyorController::startWebServer() {
   // Start the HTTP server
   webServer.begin();
   Serial.println("HTTP server started.");
}

void ConveyorController::startTicker() {
   inputCheckingTicker.attach(0.1, [this]() { updateState(); });
   LCDUpdatingTicker.attach(1, [this]() { updateLCD(); });
}

// Repeatable public access
void ConveyorController::handleClient() { webServer.handleClient(); }

void ConveyorController::updateLCD() {
   lcd.clear();
   lcd.setCursor(0, 0);
   lcd.print(WiFi.localIP());
   lcd.setCursor(0, 1);
   lcd.print("Speed:");
   lcd.setCursor(8, 1);
   lcd.print(String(conveyorSpeed) + " %");
}

void ConveyorController::updateState() {
   // Read the state of the input pins

   // docasne okomentovano abych to mohl udelat dle stavoveho diagramu

   localRemoteState = (digitalRead(PIN_IN_LOCALREMOTE) == HIGH);

   // if (localRemoteState) {
   //    if (digitalRead(PIN_OUT_LOCALREMOTE) == LOW) {
   //       digitalWrite(PIN_OUT_LOCALREMOTE, HIGH);
   //    }
   //    locOnOffState = (digitalRead(PIN_IN_ONOFF) == HIGH);
   //    locIncSpeedState = (digitalRead(PIN_IN_INCSPEED) == HIGH);
   //    locDecSpeedState = (digitalRead(PIN_IN_DECSPEED) == HIGH);
   // }
   // else {
   //    if (digitalRead(PIN_OUT_LOCALREMOTE) == HIGH) {
   //       digitalWrite(PIN_OUT_LOCALREMOTE, LOW);
   //    }
   // }
}

// Private access
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
   delay(1000);
}

void ConveyorController::mainRoute() {
   String secondsSinceStart = String(millis() / 1000);
   String response = "<p>Hello Arduino World!</p>";
   response += "<p>Time since Arduino started:";
   response += secondsSinceStart;
   response += " seconds.</p>";
   response += "<p>Local/Remote status: ";
   response += (localRemoteState) ? "Local" : "Remote";
   response += "</p>";
   response += "<div style=\"display: grid; grid-template-columns: repeat(2, 200px); grid-gap: 10px;\">\n";
   response += "<div style=\"padding: 10px; text-align: center;\">\n";
   response += "<a href=\"/incSpeedOn\">Turn inc. speed ON</a>\n";
   response += "</div>\n";
   response += "<div style=\"padding: 10px; text-align: center;\">\n";
   response += "<a href=\"/incSpeedOff\">Turn inc. speed OFF</a>\n";
   response += "</div>\n";
   response += "<div style=\"padding: 10px; text-align: center;\">\n";
   response += "<a href=\"/decSpeedOn\">Turn dec. speed ON</a>\n";
   response += "</div>\n";
   response += "<div style=\"padding: 10px; text-align: center;\">\n";
   response += "<a href=\"/decSpeedOff\">Turn dec. speed OFF</a>\n";
   response += "</div>\n";
   response += "<div style=\"padding: 10px; text-align: center;\">\n";
   response += "<a href=\"/conveyorOn\">Turn conveyor ON</a>\n";
   response += "</div>\n";
   response += "<div style=\"padding: 10px; text-align: center;\">\n";
   response += "<a href=\"/conveyorOff\">Turn conveyor OFF</a>\n";
   response += "</div>\n";
   response += "</div>\n";

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

