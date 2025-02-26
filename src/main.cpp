#include <Arduino.h>
#include "ConveyorController.h"

// Global instance of the controller
ConveyorController* conveyorController;

void setup() {
   const char* wifiNetworkName = "D1miniHotspot";
   const char* wifiNetworkPassword = "12345678910";

   conveyorController =
      new ConveyorController(wifiNetworkName, wifiNetworkPassword);

   conveyorController->initIO();
   conveyorController->initLCD();
   conveyorController->initWeb();
   conveyorController->assignRoutes();
   conveyorController->startWebServer();
   conveyorController->startTicker();
}

void loop() {
   // Getting the state of buttons is handled by ticker
   // Updating LCD is handled by ticker
   conveyorController->handleClient();
   delay(10);
}