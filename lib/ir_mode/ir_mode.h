#ifndef IR_MODE_H
#define IR_MODE_H

#include <Arduino.h>
#include <IRremote.h>
#include <motor.h>

/** Estado lógico del último comando IR reconocido (timeout → IR_STOP). */
enum IrCommand : uint8_t
{
  IR_STOP,
  IR_TURN_LEFT,
  IR_MOVE_FORWARD,
  IR_MOVE_BACKWARD,
  IR_SERVO_LEFT,
  IR_SERVO_CENTER,
  IR_SERVO_RIGHT
};

class IrMode
{
public:
  IrMode(uint8_t pinIR, MOTOR& leftMotor, MOTOR& rightMotor);

  void initializeIr();
  void loop();

private:
  uint8_t pinIR;
  MOTOR& leftMotor;
  MOTOR& rightMotor;
  IrCommand statusCommand;
  unsigned long lastIRTime;
  static const unsigned long IR_TIMEOUT = 150;
};

#endif // IR_MODE_H
