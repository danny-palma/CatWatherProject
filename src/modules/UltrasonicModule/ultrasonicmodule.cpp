#include <Arduino.h>

int SensorTank = 0; // Pin del sensor de tanque
int SensorBowl = 0; // Pin del sensor de bowl

int TANK_TRIGGER_PIN; // Pin del Trigger del sensor ultrasónico del tanque
int TANK_ECHO_PIN;     // Pin del Echo del sensor ultrasónico
int BOWL_TRIGGER_PIN; // Pin del Trigger del sensor ultrasónico del bowl
int BOWL_ECHO_PIN;     // Pin del Echo del sensor ultrasónico

void ultrasonicModuleInit(int TankTriggerPin, int TankEchoPin, int BowlTriggerPin, int BowlEchoPin)
{
    // Asignación de pines para el sensor ultrasónico del tanque
    TANK_TRIGGER_PIN = TankTriggerPin; // Pin para el Trigger del tanque
    TANK_ECHO_PIN = TankEchoPin;       // Pin para el Echo del tanque

    // Asignación de pines para el sensor ultrasónico del bowl
    BOWL_TRIGGER_PIN = BowlTriggerPin; // Pin para el Trigger del bowl
    BOWL_ECHO_PIN = BowlEchoPin;       // Pin para el Echo del bowl

    // Configuración de pines
    pinMode(TANK_TRIGGER_PIN, OUTPUT); // Configurar Trigger como salida
    pinMode(TANK_ECHO_PIN, INPUT);     // Configurar Echo como entrada

    pinMode(BOWL_TRIGGER_PIN, OUTPUT); // Configurar Trigger como salida
    pinMode(BOWL_ECHO_PIN, INPUT);     // Configurar Echo como entrada

    Serial.println("Módulo ultrasónico inicializado.");
}

void ultrasonicModuleLoop()
{
    long duration, distance;

    // Enviar un pulso de 10 microsegundos al pin Trigger
    digitalWrite(TANK_TRIGGER_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TANK_TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TANK_TRIGGER_PIN, LOW);

    // Leer el tiempo que tarda el eco en volver
    duration = pulseIn(TANK_ECHO_PIN, HIGH);

    // Calcular la distancia en centímetros
    distance = (duration / 2) / 29.1;

    Serial.print("Distancia: ");
    Serial.print(distance);
    Serial.println(" cm");
    SensorTank = distance; // Actualizar la variable global con la distancia del tanque

    // Repetir el proceso para el sensor del bowl
    digitalWrite(BOWL_TRIGGER_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(BOWL_TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(BOWL_TRIGGER_PIN, LOW);
    duration = pulseIn(BOWL_ECHO_PIN, HIGH);
    distance = (duration / 2) / 29.1;
    Serial.print("Distancia Bowl: ");
    Serial.print(distance);
    Serial.println(" cm");
    SensorBowl = distance; // Actualizar la variable global con la distancia del bowl

    delay(500); // Esperar medio segundo antes de la siguiente lectura
}