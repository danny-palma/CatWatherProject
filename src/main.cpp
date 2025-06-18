#include <Arduino.h>
#include "WifiModule.h"
#include <MFRC522.h>
#include <SPI.h> // Necesario para el RC522
#include <FirebaseClient.h>

#define SS_PIN 5   // SDA (SS) pin del RC522 conectado al GPIO 5 del ESP32
#define RST_PIN 22 // RST pin del RC522 conectado al GPIO 22 del ESP32

MFRC522 mfrc522(SS_PIN, RST_PIN); // Crea un objeto MFRC522

const char *ssid = "FibraETB0750";
const char *password = "Caro39629";

int PinReset = 0;
int btnState = false;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("Escaneando tarjeta RFID...");
  mfrc522.PCD_DumpVersionToSerial(); // Muestra la versión del MFRC522 en el monitor serial

  SetupWifi(ssid, password, PinReset);
}

void loop() {
  // Buscar nuevos tags
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial())
  {
    delay(50);
    return;
  }

  // Leer el UID del tag
  String uid_tag = "";
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    uid_tag += (mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
    uid_tag += String(mfrc522.uid.uidByte[i], HEX);
  }

  uid_tag.toUpperCase(); // Convertir a mayúsculas para consistencia

  Serial.print("UID del tag detectado: ");
  Serial.println(uid_tag);
  // Detener el PICC para evitar múltiples lecturas rápidas del mismo tag
  mfrc522.PICC_HaltA();
  // Poner el lector en modo de bajo consumo
  mfrc522.PCD_StopCrypto1();
  LoopWifi(PinReset, btnState);
}
