#ifndef SENSORSERVO_H
#define SENSORSERVO_H

#include <Arduino.h>
#include <Servo.h>

enum SENSORSERVO_STATUS
{
    TURNING,
    IDLE,
    SCANNING
};

#define MIN_ANGLE 0
#define MAX_ANGLE 180
#define ANGLE_TIME 30

class SENSORSERVO
{
public:
    SENSORSERVO(uint8_t SERVO, uint8_t TRIG, uint8_t ECHO);
    SENSORSERVO_STATUS getStatus();

    void setAngle(uint8_t angle);
    void stop();
    long getDistance();
    void updateStatus();
    void init();
    void loop();

private:
    SENSORSERVO_STATUS status = IDLE;
    SENSORSERVO_STATUS previousStatus = IDLE;
    Servo servo;
    uint8_t currentAngle, targetAngle;
    unsigned long servoDelay = 0;
    unsigned long startTurningTime = 0;
    unsigned long startScanningTime= 0;
    uint8_t pinSERVO, pinTRIG, pinECHO;

    void updateOutputs();
};

#endif
