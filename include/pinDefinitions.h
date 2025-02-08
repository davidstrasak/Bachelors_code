#ifndef PINDEFINITONS
#define PINDEFINITONS

// ----------------------
// INPUT PIN DEFINITIONS
// ----------------------
#define PIN_IN_ONOFF \
   A0  // Watches for the physical button that turns the conveyor on
#define PIN_IN_INCSPEED \
   D0                  // Watches for the physical button that increases speed
#define PIN_IN_SCL D1  // SCL for the LCD
#define PIN_IN_SDA D2  // SDA for the LCD
#define PIN_IN_DECSPEED \
   D3  // Watches for the physical button that decreases speed
#define PIN_IN_LOCALREMOTE \
   D4  // Watches for the physical switch that toggles between local and remote
       // control

// ----------------------
// OUTPUT PIN DEFINITIONS
// ----------------------
#define PIN_OUT_DECSPEED D5  // Controls the conveyor speed decrease
#define PIN_OUT_ONOFF D6     // Controls the conveyor on/off
#define PIN_OUT_LOCALREMOTE \
   D7  // Switches the Type C relays between local and remote control
#define PIN_OUT_INCSPEED D8  // Controls the conveyor speed increase
#endif