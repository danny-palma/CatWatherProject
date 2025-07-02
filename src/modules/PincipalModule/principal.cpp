#include "principal.h"
#include "../RFIDModule/rfidmodule.h"
#include "../ultrasonicmodule/ultrasonicmodule.h"

#define pinTankActivator 26
#define pinBowlActivator 34

void verificaragua(){
    // Aquí iría la lógica para verificar el nivel de agua
    Serial.println("Verificando nivel de agua...");

    if(SensorTank > 2){
        Serial.println("Nivel de agua bajo, por favor rellene el tanque.");
    } else if(SensorTank < 2 && SensorBowl > 2){
        Serial.println("Nivel de agua en bowl bajo, por favor rellene el bowl.");
    } else {
        Serial.println("Nivel de agua adecuado.");
    }
    // Simulación de verificación
    delay(1000);
    Serial.println("Nivel de agua verificado.");
}


void principalloop(){
    verificaragua();
    if(tagDetected){
        Serial.println("Tarjeta RFID detectada.");
        // Aquí iría la lógica para manejar la tarjeta RFID detectada
        tagDetected = false; // Reiniciar el estado de detección
        pinMode(pinTankActivator, OUTPUT);
        digitalWrite(pinTankActivator, HIGH); // Activar el tanque
        Serial.println("Tanque activado.");
        delay(200); // Esperar 2 segundos

    } else {
        Serial.println("Esperando tarjeta RFID...");
    }

}