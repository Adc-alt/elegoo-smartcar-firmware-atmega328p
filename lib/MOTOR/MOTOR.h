#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>
#include <ElegooSmartCar.h>

enum MOTOR_STATUS
{
    FORWARD,
    REVERSE,
    FORCE_STOP, // Para y frena asignamos el PWM a 0 por eso frena
    FREE_STOP   // Para y ya
};

#define MIN_PWM 35 // A partir de aquí empieza a moverse
#define MAX_PWM 255

class MOTOR
{
public:
    MOTOR(uint8_t IN1, uint8_t PWM, uint8_t stby); // Declaración del constructor, declaro los parametros que necesito meter para cuando instancie la clase
    MOTOR_STATUS getstatus();

    void forward(uint8_t vel);
    void reverse(uint8_t vel);
    void freeStop();
    void forceStop();

    void tick();

private:
    MOTOR_STATUS status = FORCE_STOP;
    uint8_t currentPwm = 0;
    uint8_t pinIN1, pinPWM, pinSTBY;
    // bool in1,in2 = false;

    void updateOutputs();
};

#endif