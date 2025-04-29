#ifndef SENSORSERVO_H
#define SENSORSERVO_H

#include <Arduino.h>
#include <Servo.h>

Servo servo = Servo();

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
    void getDistance();
    void updateStatus();
    void loop();

private:
    SENSORSERVO_STATUS status = IDLE;
    SENSORSERVO_STATUS previousStatus = IDLE;
    Servo servo;
    uint8_t currentAngle, targetAngle;
    int servoDelay = 0;
    int startTurnintTime = 0;
    uint8_t pinSERVO, pinTRIG, pinECHO;

    void updateOutputs();

    // Estructura para el temporizador
    struct Timer
    {
        unsigned long startTime;
        bool isRunning;
        void start()
        {
            startTime = millis();
            isRunning = true;
        }
        bool hasElapsed(unsigned long duration)
        {
            return isRunning && (millis() - startTime >= duration);
        }
        void stop()
        {
            isRunning = false;
        }
    } timer;
};

#endif
