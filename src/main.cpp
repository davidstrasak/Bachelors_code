#include <Arduino.h>
#include "ConveyorController.h"

// Global instance of the controller
ConveyorController* conveyorController;

void setup() {
   const char* wifiNetworkName = "TP-Link_83CA";
   const char* wifiNetworkPassword = "65362280";

   conveyorController =
      new ConveyorController(wifiNetworkName, wifiNetworkPassword);

   conveyorController->initIO();
   conveyorController->initLCD();
   conveyorController->initWeb();
   conveyorController->assignRoutes();
   conveyorController->startWebServer();
}

void loop() {
   conveyorController->handleClient();
   conveyorController->updateLCD();
   delay(10);
}