#ifndef RFID_MODULE_H
#define RFID_MODULE_H

#include <MFRC522.h>
void RFIDSetup(int RFID_SS_PIN, int RFID_RST_PIN);
void RFIDLoop();

extern bool tagDetected;
#endif