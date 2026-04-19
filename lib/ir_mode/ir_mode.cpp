#include "ir_mode.h"

IrMode::IrMode(uint8_t pinIR, MOTOR& leftMotor, MOTOR& rightMotor)
    : pinIR(pinIR), leftMotor(leftMotor), rightMotor(rightMotor), statusCommand(IR_STOP), lastIRTime(0)
{
}

void IrMode::initializeIr()
{
  pinMode(pinIR, INPUT);
  IrReceiver.begin(pinIR, ENABLE_LED_FEEDBACK);
  delay(50);
}

void IrMode::loop()
{
  if (IrReceiver.decode())
  {
    lastIRTime = millis();

    switch (IrReceiver.decodedIRData.decodedRawData)
    {
      case 0xF609FF00:
        rightMotor.forward(100);
        // Serial.println("TURN_RIGHT");
        break;
      case 0xF807FF00:
        this->statusCommand = IR_TURN_LEFT;
        // Serial.println("TURN_LEFT");
        break;
      case 0xBF40FF00:
        this->statusCommand = IR_MOVE_FORWARD;
        // Serial.println("MOVE_FORWARD");
        break;
      case 0xE619FF00:
        this->statusCommand = IR_MOVE_BACKWARD;
        // Serial.println("MOVE_BACKWARD");
        break;
      case 0xF30CFF00:
        this->statusCommand = IR_SERVO_LEFT;
        // Serial.println("SERVO_LEFT");
        break;
      case 0xE718FF00:
        this->statusCommand = IR_SERVO_CENTER;
        // Serial.println("SERVO_CENTER");
        break;
      case 0xA15EFF00:
        this->statusCommand = IR_SERVO_RIGHT;
        // Serial.println("SERVO_RIGHT");
        break;
      case 0xFD00FF00:
      default:
        // Serial.print("Código no reconocido: 0x");
        // Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);
        // NO cambiar el comando si no reconocemos la señal
        break;
    }
    IrReceiver.resume();
  }
  else
  {
    // Auto-reset después de timeout
    if (millis() - lastIRTime > IR_TIMEOUT && lastIRTime != 0)
    {
      this->statusCommand = IR_STOP;
    }
  }
}