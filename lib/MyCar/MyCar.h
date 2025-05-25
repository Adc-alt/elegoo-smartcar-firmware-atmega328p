#ifndef MYCAR_H
#define MYCAR_H

#include <Arduino.h>
#include "../SensorServo/SensorServo.h"
#include "../MPU/MPUControl.h"
#include "../Battery/Battery.h"
#include "../LineTracking/LineTracking.h"
#include "../LEDRGB/LEDRGB.h"
#include "../Motor/Motor.h"

enum MYCAR_STATUS
{
    MODO_MANUAL,
    MODO_EMERGENCY,
    MODO_LIFTED,
    MODO_LINE_FOLLOWING,
    MODO_OBJECT_FOLLOWING,
    MODO_OBSTACLE_AVOIDANCE,
    MODO_FACE_FOLLOWING,
    MODO_BALL_FOLLOWING,
    MODO_STOPPED,
};

class MyCar
{
public:
    MyCar(SENSORSERVO &sensorServo, MPUControl &mpuControl, BATTERY &battery,
          LINE_TRACKING &lineTracking, LED_RGB &ledRGB, MOTOR &motor);

    // Métodos públicos principales
    void loop();
    void begin();

    // Getters
    void setStatus(MYCAR_STATUS status);
    MYCAR_STATUS getStatus();

private:
    // 1. Variables de estado
    MYCAR_STATUS curretstatus = MODO_STOPPED;
    MYCAR_STATUS previousStatus = MODO_STOPPED;
    MYCAR_STATUS nextStatus = MODO_STOPPED;

    // Métodos privados auxiliares
    void updateStatus();
    void updateOutputs();

    // Punteros a componentes
    SENSORSERVO *sensorServo;
    MPUControl *mpuControl;
    BATTERY *battery;
    LINE_TRACKING *lineTracking;
    LED_RGB *ledRGB;
    MOTOR *motor;
};

#endif
