#include <Arduino.h>
#include "WifiModule.h"

const char *ssid = "FibraETB0750";
const char *password = "Caro39629";

int PinReset = 0;
int btnState = false;

void setup() {
  Serial.begin(9600);
  
  SetupWifi(ssid, password, PinReset);
}

void loop() {
  LoopWifi(PinReset, btnState);
}
