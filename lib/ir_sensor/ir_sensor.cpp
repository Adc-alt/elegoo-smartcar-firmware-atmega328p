#include "ir_sensor.h"

#include <IRremote.h>

IrSensor::IrSensor(uint8_t pinIR) : pinIR(pinIR)
{
}

void IrSensor::begin()
{
  IrReceiver.begin(pinIR, ENABLE_LED_FEEDBACK);
  delay(50);
}

const char* IrSensor::getIrCommand()
{
  if (IrReceiver.decode())
  {
    irNew      = true;
    irRaw      = IrReceiver.decodedIRData.decodedRawData;
    lastIRTime = millis();

    switch (IrReceiver.decodedIRData.decodedRawData)
    {
      case 0xBC43FF00:
        irCommand = "right";
        break;
      case 0xBB44FF00:
        irCommand = "left";
        break;
      case 0xB946FF00:
        irCommand = "forward";
        break;
      case 0xEA15FF00:
        irCommand = "backward";
        break;
      case 0xE916FF00:
        irCommand = "servo_left";
        break;
      case 0xE619FF00:
        irCommand = "servo_center";
        break;
      case 0xF20DFF00:
        irCommand = "servo_right";
        break;
      case 0xFD00FF00:
      default:
        irCommand = "stop";
        break;
    }

    IrReceiver.resume();
  }
  else
  {
    // timeout: cambia a "stop" solo si ha pasado el timeout
    // PERO solo si antes había una señal (lastIRTime != 0)
    if (lastIRTime != 0 && (millis() - lastIRTime > IR_TIMEOUT_MS))
    {
      irCommand  = "stop";
      lastIRTime = 0; // Resetear para detectar próxima señal
    }
  }
  return irCommand;
}
