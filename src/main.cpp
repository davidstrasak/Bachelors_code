#include <Arduino.h>
/*
* Using LCD screen with wemos D1 Mini and Arduino
* Written by Ahmad Shamshiri on Aug 01, 2019
* at 17:42 in Ajax, Ontario, Canada
* for Robojax.com
* Watch video instruction for this code: https://youtu.be/mRAgS-CdURU
*
Get this code and other Arduino codes from Robojax.com
If you found this tutorial helpful, please support me so I can continue creating
content like this. You can support me on Patreon http://robojax.com/L/?id=63
or make donation using PayPal http://robojax.com/L/?id=64

*/

// start of settings for LCD1602 with I2C
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);
// end of settings for LCD1602 with I2C

int count = 0;
void setup() {

   pinMode(A0, INPUT);
   pinMode(D0, INPUT);
   // pinMode(D1, OUTPUT); // D1 is used for I2C
   // pinMode(D2, OUTPUT); // D2 is used for I2C
   pinMode(D3, INPUT);
   pinMode(D4, INPUT);
   pinMode(D5, OUTPUT);
   pinMode(D6, OUTPUT);
   pinMode(D7, OUTPUT);
   pinMode(D8, OUTPUT);
   pinMode(LED_BUILTIN, OUTPUT);

   digitalWrite(D5, LOW);
   digitalWrite(D6, LOW);
   digitalWrite(D7, LOW);
   digitalWrite(D8, LOW);

   lcd.init();
   lcd.backlight();
   lcd.print("Uz to jede");
   lcd.setCursor(0, 1);
   lcd.print("Demo");
   delay(2000);
}

void loop() {
   // Robojax D1 Mini LCD-1602-I2C
   lcd.clear();  // clear previous values from screen
   lcd.print("Funguje to.");
   lcd.setCursor(0, 1);
   lcd.print("Counting:");
   lcd.setCursor(11, 1);
   lcd.print(count);
   count++;
   delay(200);

   // Basic LED blink on D5 pin
   digitalWrite(LED_BUILTIN, HIGH);
   delay(500);
   digitalWrite(LED_BUILTIN, LOW);
   delay(500);
}
