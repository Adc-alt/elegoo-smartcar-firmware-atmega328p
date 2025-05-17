#ifndef SENSORSERVO_H
#define SENSORSERVO_H

#include <Arduino.h>
#include <Servo.h>
#include <HC-SR04.h>

// #define USE_HEAD

enum SENSORSERVO_STATUS
{
    IDLE,
    // REPOSO: Servo parado y HC-SR04 en reposo
    TURNING,
    // GIRANDO: Servo girando hasta el proximo punto para escanear
    SCANNING,
    // BARRIENDO: Servo girando cada SCANNING_STEP grados y escanenado, obtiene minimo y maximos
    SEARCHING,
    // BUSCANDO: Servo girando cada SEARCHING_STEP grados y parandose cuando encuentre algo a menos de SEARCHINV_THEEHOLD
};

#define SCANNING_STEP 3

#define SEARCHING_STEP 5
#define SEARCHING_THRESHOOLD 20

#define ANGLE_TIME 250

#define MIN_ANGLE 10
#define FRONT_ANGLE 90
#define MAX_ANGLE 150

class SENSORSERVO
{
public:
    SENSORSERVO(HCSR04 &sensor, Servo &servo);

    SENSORSERVO_STATUS getStatus();

    void loop();

    void startScanning();  // Barrido-> Leer todo y dar minimos y maximos con sus respectivos angulos
    void startSearching(); // Busqueda-> Girar y parar cuando encuentre un minimo
    void stop();
    // Servo
    void setAngle(uint8_t angle);
    void setAngle(uint8_t angle, SENSORSERVO_STATUS nextStatus);

    int getSearchAngle();

    // Ultrasonido

private:
    SENSORSERVO_STATUS status = IDLE;
    SENSORSERVO_STATUS previousStatus = IDLE;
    SENSORSERVO_STATUS nextStatus = IDLE;

    // Servo
    Servo *servo;
    HCSR04 *sensor;

    unsigned long servoDelay = 0;
    unsigned long startTurningTime = 0;
    uint8_t currentAngle, targetAngle;

    int objectAngle = -1; //-1 no encontrado si no valor del angulo con minima distancia
    int nextSearchAngle = -1;
    int searchIndex = 0;

    void updateStatus();
    void updateOutputs();
};

String statusToString(SENSORSERVO_STATUS status);


#endif
