#ifndef SENSORSERVO_H
#define SENSORSERVO_H

#include <Arduino.h>
#include <Servo.h>

enum SENSORSERVO_STATUS
{
    TURNING_LEFT,
    TURNING_RIGHT,
    STOP,
    SCANING
};

class SENSORSERVO
{
    public:
        SENSORSERVO(uint8_t SERVO,uint8_t TRIG,uint8_t ECHO);
        SENSORSERVO_STATUS getStatus();

        void setAngle();
        void turnLeft();        
        void turnRight();
        void stop();
        void getDistance();
    
    private:
        SENSORSERVO_STATUS status=STOP;
        uint8_t currentAngle;
        uint8_t pinSERVO,pinTRIG,pinECHO;
        void updateState();
};


#endif
