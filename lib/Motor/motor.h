#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>
#include <hardwarePins.h>

enum MOTOR_STATUS
{
    FORWARD,
    REVERSE,
    FORCE_STOP,
    FREE_STOP
};

#define MIN_PWM 38
#define MAX_PWM 255


class MOTOR
{
    public:
        MOTOR(uint8_t IN1, uint8_t PWM, uint8_t stby);//Declaración del constructor, declaro los parametros que necesito meter para cuando instancie la clase        
        MOTOR_STATUS getstatus();

        void forward(uint8_t vel);
        void reverse(uint8_t vel);
        void freeStop();
        void forceStop();

        void tick();
    private:
        MOTOR_STATUS status = FORCE_STOP;
        uint8_t currentPwm= 0;
        uint8_t pinIN1,pinIN2,pinPWM,pinSTBY;
        bool in1,in2 = false;

        void updateOutputs();
};


#endif