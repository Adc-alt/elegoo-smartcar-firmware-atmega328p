#ifdef IR_MODE
#define IR_MODE

#include <Arduino.h>
#include <IRremote.h>
#include <motor.h>

class IrMode
{
public:
  IrMode(uint8_t pinIR, MOTOR& leftMotor, MOTOR& rightMotor);
  void inizializeIr();

private:
  void decode();
  void handleMessage();
}
