#ifndef CONVEYORCONTROLLER_H
#define CONVEYORCONTROLLER_H

#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <LiquidCrystal_I2C.h>
#include <WiFiClient.h>
#include <Ticker.h>
#include "pinDefinitions.h"

class ConveyorController {
public:
  ConveyorController(const char* wifiNetworkName,
    const char* wifiNetworkPassword);

  // Public access initializations
  void initIO();
  void initLCD();
  void initWeb();
  void assignRoutes();
  void startWebServer();
  void startTicker();

  // Repeatable public access
  void handleClient();
  void updateLCD();
  void updateState();

private:
  // WiFi credentials
  const char* wifiNetworkName;
  const char* wifiNetworkPassword;

  // Web server
  ESP8266WebServer webServer;

  // LCD
  LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);

  // Ticker
  Ticker inputCheckingTicker;
  Ticker LCDUpdatingTicker;

  // States
  bool localRemoteState = false;
  bool locOnOffState = false;
  bool locIncSpeedState = false;
  bool locDecSpeedState = false;
  bool remOnOffState = false;
  bool remIncSpeedState = false;
  bool remDecSpeedState = false;

  // Conveyor speed
  int conveyorSpeed = 0;

  // Private access
  void LCDWaitingForConnection();
  void mainRoute();
  void unknownRouteResponse();
  void handleLocalControl();
  void handleConveyorOnLocally();
  void handleConveyorOffLocally();
  void handleRemoteControl();
  void handleConveyorOnRemotely();
  void handleConveyorOffRemotely();
  void increaseConveyorSpeed();
  void decreaseConveyorSpeed();
  void setPinIfDifferent(int pin, int value, bool condition);
};

#endif