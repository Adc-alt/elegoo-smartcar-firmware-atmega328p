
#ifndef SENSORSERVO_H
#define SENSORSERVO_H

#include <Arduino.h>
#include <Servo.h>
#include "HCSR04.h"

// 1. Enums y constantes públicas
enum SENSORSERVO_STATUS
{
    IDLE,     // REPOSO: Servo parado y HC-SR04 en reposo
    TURNING,  // GIRANDO: Servo girando hasta el proximo punto para escanear
    SCANNING, // BARRIENDO: Servo girando cada SCANNING_STEP grados y escanenado
    SEARCHING // BUSCANDO: Servo girando cada SEARCHING_STEP grados
};

enum SCANNING_STATE
{
    SCAN_START,   // ESCANEO : Inicio
    SCAN_LEFT,    // ESCANEO : A la izquierda
    SCAN_CENTER,  // ESCANEO : Al centro
    SCAN_RIGHT,   // ESCANEO : A la derecha
    SCAN_COMPLETE // ESCANEO : Completo
};

// 2. Constantes de configuración
#define MIN_ANGLE 10
#define FRONT_ANGLE 90
#define MAX_ANGLE 150
#define SEARCHING_STEP 20
#define SEARCHING_THRESHOOLD 20
#define ANGLE_TIME 30

class SENSORSERVO
{
public:
    // 3. Constructor
    SENSORSERVO(HCSR04 &sensor, Servo &servo);

    // 4. Métodos públicos principales
    void loop();
    void startScanning();
    void startSearching();
    void stop();

    // 5. Getters
    SENSORSERVO_STATUS getStatus();
    int getSearchAngle();

    // 6. Setters
    void setAngle(uint8_t angle);
    void setAngle(uint8_t angle, SENSORSERVO_STATUS nextStatus);

private:
    // 7. Constantes privadas
    static const int NO_OBJECT_FOUND = -1;

    // 8. Variables de estado
    SENSORSERVO_STATUS status = IDLE;
    SENSORSERVO_STATUS previousStatus = IDLE;
    SENSORSERVO_STATUS nextStatus = IDLE;
    SCANNING_STATE scanningState = SCAN_CENTER;

    // 9. Punteros a componentes
    Servo *servo;
    HCSR04 *sensor;

    // 10. Variables de control
    unsigned long servoDelay = 0;
    unsigned long startTurningTime = 0;
    uint8_t currentAngle, targetAngle;
    uint8_t minDistance, maxDistance, middleDistance;
    int objectAngle = NO_OBJECT_FOUND;
    int nextSearchAngle = MIN_ANGLE;
    int searchIndex = 0;
    unsigned long elapsed = 0;

    // 11. Métodos privados de actualización
    void updateStatus();
    void updateOutputs();
    unsigned long calculateServoDelay(uint8_t currentAngle, uint8_t targetAngle);
};

// 12. Funciones auxiliares
String statusToString(SENSORSERVO_STATUS status);

#endif