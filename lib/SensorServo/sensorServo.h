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

class SENSORSERVO
{
    public:
        SENSORSERVO(uint8_t SERVO,uint8_t TRIG,uint8_t ECHO);
        SENSORSERVO_STATUS getStatus();

        void setAngle(uint8_t angle);
        void stop();
        void getDistance();
    
    private:
        Servo servo;
        SENSORSERVO_STATUS status=IDLE;
        uint8_t currentAngle;
        uint8_t targetAngle;
        uint8_t pinSERVO,pinTRIG,pinECHO;
        
        long updateOutputs();
        // void updateOutputs();
        


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
            void stop() { 
                isRunning = false; 
            }
        } timer;
};


#endif
