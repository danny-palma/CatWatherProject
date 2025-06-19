#include <Arduino.h>
#include "modules/wifimodule/WifiModule.h"
#include "modules/rfidmodule/rfidmodule.h"
#include "modules/UltrasonicModule/ultrasonicmodule.h"
#include "modules/FirebaseModule/FirebaseModule.h"
#include <FirebaseClient.h>


// definicion de pines
#define RESET_PIN 0 // Pin de reset del ESP32, conectado al GPIO 0

#define RFID_SS_PIN 5   // SDA (SS) pin del RC522 conectado al GPIO 5 del ESP32
#define RFID_RST_PIN 22 // RST pin del RC522 conectado al GPIO 22 del ESP32

#define TANK_TRIGGER_PIN 13 // Pin del Trigger del sensor ultrasónico del tanque
#define TANK_ECHO_PIN 12 // Pin del Echo del sensor ultrasónico del tanque
#define BOWL_TRIGGER_PIN 33 // Pin del Trigger del sensor ultrasónico del bowl
#define BOWL_ECHO_PIN 32 // Pin del Echo del sensor ultrasónico del bowl

const char *ssid = "Danny's A25";
const char *password = "danny2017";

int btnState = false;

void setup()
{
  Serial.begin(9600);
  // Crear semáforos para cada módulo
  static SemaphoreHandle_t wifiSemaphore = xSemaphoreCreateBinary();
  static SemaphoreHandle_t firebaseSemaphore = xSemaphoreCreateBinary();
  static SemaphoreHandle_t rfidSemaphore = xSemaphoreCreateBinary();
  static SemaphoreHandle_t ultrasonicSemaphore = xSemaphoreCreateBinary();

  // Modificar las tareas para que den la señal cuando terminen
  xTaskCreate([](void *) {
    ultrasonicModuleInit(TANK_TRIGGER_PIN, TANK_ECHO_PIN, BOWL_TRIGGER_PIN, BOWL_ECHO_PIN);
    xSemaphoreGive(ultrasonicSemaphore);
    vTaskDelete(NULL);
  }, "InitUltrasonic", 2048, NULL, 1, NULL);

  xTaskCreate([](void *) {
    SetupFirebase("your_firebase_host", "your_firebase_auth");
    xSemaphoreGive(firebaseSemaphore);
    vTaskDelete(NULL);
  }, "InitFirebase", 4096, NULL, 1, NULL);

  xTaskCreate([](void *) {
    SetupWifi(ssid, password, RESET_PIN);
    xSemaphoreGive(wifiSemaphore);
    vTaskDelete(NULL);
  }, "InitWifi", 2048, NULL, 1, NULL);

  xTaskCreate([](void *) {
    RFIDSetup(RFID_SS_PIN, RFID_RST_PIN);
    xSemaphoreGive(rfidSemaphore);
    vTaskDelete(NULL);
  }, "InitRFID", 2048, NULL, 1, NULL);

  // Esperar a que todos los módulos se inicialicen
  xSemaphoreTake(wifiSemaphore, portMAX_DELAY);
  xSemaphoreTake(firebaseSemaphore, portMAX_DELAY);
  xSemaphoreTake(rfidSemaphore, portMAX_DELAY);
  xSemaphoreTake(ultrasonicSemaphore, portMAX_DELAY);
}

// Definir tareas
void ultrasonicTask(void *parameter) {
  for(;;) {
    ultrasonicModuleLoop();
    vTaskDelay(pdMS_TO_TICKS(100)); // Delay de 100ms
  }
}

void firebaseTask(void *parameter) {
  for(;;) {
    LoopFirebase();
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void rfidTask(void *parameter) {
  for(;;) {
    RFIDLoop();
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void wifiTask(void *parameter) {
  for(;;) {
    LoopWifi(RESET_PIN, btnState);
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void loop() {
  // Crear tareas solo una vez
  static bool tasksCreated = false;
  if (!tasksCreated) {
    xTaskCreate(ultrasonicTask, "Ultrasonic", 2048, NULL, 1, NULL);
    xTaskCreate(firebaseTask, "Firebase", 4096, NULL, 1, NULL);
    xTaskCreate(rfidTask, "RFID", 2048, NULL, 1, NULL);
    xTaskCreate(wifiTask, "WiFi", 2048, NULL, 1, NULL);
    tasksCreated = true;
  }
  vTaskDelay(pdMS_TO_TICKS(1000)); // Mantener el loop principal vivo
}
