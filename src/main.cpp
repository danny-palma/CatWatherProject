#include <Arduino.h>
#include "modules/rfidmodule/rfidmodule.h"
#include "modules/UltrasonicModule/ultrasonicmodule.h"
#include "modules/FirebaseModule/example.h" // Importa las funciones de ejemplo de Firebase
#include <ArduinoJson.h>
// definicion de pines
#define RESET_PIN 0 // Pin de reset del ESP32, conectado al GPIO 0

#define RFID_SS_PIN 5   // SDA (SS) pin del RC522 conectado al GPIO 5 del ESP32
#define RFID_RST_PIN 22 // RST pin del RC522 conectado al GPIO 22 del ESP32

#define TANK_TRIGGER_PIN 13 // Pin del Trigger del sensor ultrasónico del tanque
#define TANK_ECHO_PIN 12    // Pin del Echo del sensor ultrasónico del tanque
#define BOWL_TRIGGER_PIN 33 // Pin del Trigger del sensor ultrasónico del bowl
#define BOWL_ECHO_PIN 32    // Pin del Echo del sensor ultrasónico del bowl

const char *ssid = "Danny's A25";
const char *password = "danny2017";

int btnState = false;

void setup()
{
  Serial.begin(9600);
  ultrasonicModuleInit(TANK_TRIGGER_PIN, TANK_ECHO_PIN, BOWL_TRIGGER_PIN, BOWL_ECHO_PIN);

  RFIDSetup(RFID_SS_PIN, RFID_RST_PIN);

  firebaseSetup(ssid, password);
}

// Definir tareas
void ultrasonicTask(void *parameter)
{
  for (;;)
  {
    ultrasonicModuleLoop();
    vTaskDelay(pdMS_TO_TICKS(100)); // Delay de 100ms
  }
}

void rfidTask(void *parameter)
{
  for (;;)
  {
    RFIDLoop();
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void loop()
{
  // Crear tareas solo una vez
  static bool tasksCreated = false;
  if (!tasksCreated)
  {
    // Crear tareas principales
    xTaskCreate(ultrasonicTask, "Ultrasonic", 2048, NULL, 1, NULL);
    xTaskCreate(rfidTask, "RFID", 2048, NULL, 1, NULL);
    tasksCreated = true;
  }
  firebaseLoop(); // Llamar a la función de ejemplo de Firebase
  vTaskDelay(pdMS_TO_TICKS(1000)); // Mantener el loop principal vivo
}
