#ifndef MYCAR_H
#define MYCAR_H

#include <Arduino.h>
#include "SensorServo/SensorServo.h"
#include "MPU/MPUControl.h"
#include "Battery/Battery.h"
#include "LineTracking/LineTracking.h"
#include "LEDRGB/LEDRGB.h"
#include "Motor/motor.h"
#include "IrControl/IrControl.h"

enum MYCAR_STATUS
{
    MODO_STOPPED,
    MODO_MANUAL,
    MODO_EMERGENCY,
    MODO_LIFTED,
    MODO_LINE_FOLLOWING,
    MODO_OBJECT_FOLLOWING,
    MODO_OBSTACLE_AVOIDANCE,
    MODO_FACE_FOLLOWING,
    MODO_BALL_FOLLOWING
};

class MyCar
{
public:
    MyCar(SENSORSERVO &sensorServo, MPUControl &mpuControl, BATTERY &battery,
          LINE_TRACKING &lineTracking, LED_RGB &ledRGB, MOTOR &leftMotor, MOTOR &rightMotor, IRCONTROL &irControl);

    // Métodos públicos principales
    void loop();
    void inizializeMyCar();
    void startManual();
    void startEmergency();
    void startLifted();
    void startLineFollowing();
    void startObjectFollowing();
    void startFaceFollowing();
    void startBallFollowing();

    // Getters
    MYCAR_STATUS getCurrentStatus();
    void setObstacleAvoidance(bool value) { isobstacleAvoidanceTrue = value; }

private:
    // 1. Variables de estado
    MYCAR_STATUS currentStatus = MODO_STOPPED;
    MYCAR_STATUS previousStatus = MODO_STOPPED;
    MYCAR_STATUS nextStatus = MODO_STOPPED;

    // 2. Variables de control
    bool isobstacleAvoidanceTrue = false; // Flag para controlar el modo de evasión de obstáculos

    // Métodos privados auxiliares
    void updateStatus();
    void updateOutputs();
    void readInputs();

    // Punteros a componentes
    SENSORSERVO *sensorServo;
    MPUControl *mpuControl;
    BATTERY *battery;
    LINE_TRACKING *lineTracking;
    LED_RGB *ledRGB;
    MOTOR *leftMotor;
    MOTOR *rightMotor;
    IRCONTROL *irControl;
};
// 1. Funciones auxiliares
String statusToString(MYCAR_STATUS status);

#endif
