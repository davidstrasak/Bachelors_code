#ifndef CONVEYORCONTROLLER_H
#define CONVEYORCONTROLLER_H

#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <LiquidCrystal_I2C.h>
#include <WiFiClient.h>
#include <Wire.h>
#include "pinDefinitions.h"

class ConveyorController {
public:
  ConveyorController(const char* wifiNetworkName,
    const char* wifiNetworkPassword);

  void initIO();
  void initLCD();
  void initWeb();
  void assignRoutes();
  void startWebServer();
  void handleClient();
  void updateLCD();
  void updateState();

private:
  // WiFi credentials
  const char* wifiNetworkName;
  const char* wifiNetworkPassword;

  // Web server
  ESP8266WebServer webServer = ESP8266WebServer(80);

  // LCD
  LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);

  // Speed of the conveyor
  int conveyorSpeed = 0;

  // TRUE or FALSE state if the conveyor is controlled locally or remotely
  bool locRemState = false;

  // TRUE or FALSE state if the conveyor is speeding up or no
  bool incSpeedState = false;

  // TRUE or FALSE state if the conveyor is slowing down or no
  bool decSpeedState = false;

  // TRUE or FALSE state if the conveyor is ON or OFF
  bool onOffState = false;

  // Route handler for the main page
  void mainRoute();

  // Route handler for unknown pages
  void unknownRouteResponse();

  void LCDWaitingForConnection();



};

#endif