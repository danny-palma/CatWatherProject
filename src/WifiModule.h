#ifndef WIFI_MODULE_H
#define WIFI_MODULE_H

#include <Arduino.h>

void SetupWifi(const char *SSID, const char *Password, int PinWifiDisconnect);

void LoopWifi(int PinReset, int &btnState);

#endif