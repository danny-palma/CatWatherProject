#include "rfidmodule.h"

#include <Arduino.h>
#include <MFRC522.h>
#include <SPI.h> // Necesario para el RC522

MFRC522 mfrc522; // Crea un objeto MFRC522

void RFIDSetup(int RFID_SS_PIN, int RFID_RST_PIN)
{
    mfrc522 = MFRC522(RFID_SS_PIN, RFID_RST_PIN); // Inicializa el objeto MFRC522 con los pines SS y RST
    SPI.begin();
    mfrc522.PCD_Init();
    Serial.println("Escaneando tarjeta RFID...");
    mfrc522.PCD_DumpVersionToSerial(); // Muestra la versión del MFRC522 en el monitor serial
}


void RFIDLoop()
{
    // Verifica si hay una tarjeta presente
    if (!mfrc522.PICC_IsNewCardPresent()) {
        return; // No hay tarjeta presente, salir de la función
    }

    // Selecciona la tarjeta
    if (!mfrc522.PICC_ReadCardSerial()) {
        return; // No se pudo leer la tarjeta, salir de la función
    }

    // Muestra el UID de la tarjeta en el monitor serial
    Serial.print("UID de la tarjeta: ");
    for (byte i = 0; i < mfrc522.uid.size; i++) {
        Serial.print(mfrc522.uid.uidByte[i], HEX);
        Serial.print(" ");
    }
    Serial.println();
    //muestra la información de la tarjeta
    Serial.print("Tipo de tarjeta: ");
    switch (mfrc522.PICC_GetType(mfrc522.uid.sak)) {
        case MFRC522::PICC_TYPE_MIFARE_MINI:
            Serial.println("MIFARE Mini");
            break;
        case MFRC522::PICC_TYPE_MIFARE_1K:
            Serial.println("MIFARE 1K");
            break;
        case MFRC522::PICC_TYPE_MIFARE_4K:
            Serial.println("MIFARE 4K");
            break;
        case MFRC522::PICC_TYPE_MIFARE_UL:
            Serial.println("MIFARE Ultralight");
            break;
        case MFRC522::PICC_TYPE_MIFARE_PLUS:
            Serial.println("MIFARE Plus");
            break;
        case MFRC522::PICC_TYPE_MIFARE_DESFIRE:
            Serial.println("MIFARE DESFire");
            break;
        default:
            Serial.println("Tipo desconocido");
            break;
    }
    // muestra lo que contiene la tarjeta
    Serial.print("Contenido de la tarjeta: ");
    for (byte i = 0; i < mfrc522.uid.size; i++) {
        Serial.print(mfrc522.uid.uidByte[i], HEX);
        if (i < mfrc522.uid.size - 1) {
            Serial.print(":");
        }
    }
    // Finaliza la lectura de la tarjeta
    mfrc522.PICC_HaltA();
    // Poner el lector en modo de bajo consumo
    mfrc522.PCD_StopCrypto1();
}
