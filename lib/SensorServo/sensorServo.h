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

    void updateStatus();
    void init();
    void loop();

    // Servo
    void setAngle(uint8_t angle);

    // Ultrasonido
    bool isScanningStart;
    bool isScanningStop;
    uint8_t getDistance();

private:
    SENSORSERVO_STATUS status = IDLE;
    SENSORSERVO_STATUS previousStatus = IDLE;

    // Servo
    Servo servo;
    uint8_t currentAngle, targetAngle;
    unsigned long servoDelay = 0;
    unsigned long startTurningTime = 0;

    // Ultrasonic
    uint8_t pinSERVO, pinTRIG, pinECHO;
    uint8_t distance;
    unsigned long startScanningTime = 0;
    unsigned long startTime = 0;
    bool started = false;
    bool measured = false;

    const int intervalo = 50; // tiempo entre mediciones (ms)
    const int numMedidas = 4;
    float medidas[4] = {0}; // 4 medidas
    int indice = 0;

    void updateOutputs();
};

#endif
