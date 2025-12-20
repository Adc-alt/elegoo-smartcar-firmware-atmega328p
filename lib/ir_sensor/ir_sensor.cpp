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

void IrSensor::update(TelemetryFrame& frame)
{
  if (IrReceiver.decode())
  {
    frame.ir_new = true;
    frame.ir_raw = IrReceiver.decodedIRData.decodedRawData;
    lastIRTime   = millis();

    switch (IrReceiver.decodedIRData.decodedRawData)
    {
      case 0xBC43FF00:
        frame.ir_data = "right";
        break;
      case 0xBB44FF00:
        frame.ir_data = "left";
        break;
      case 0xB946FF00:
        frame.ir_data = "forward";
        break;
      case 0xEA15FF00:
        frame.ir_data = "backward";
        break;
      case 0xE916FF00:
        frame.ir_data = "servo_left";
        break;
      case 0xE619FF00:
        frame.ir_data = "servo_center";
        break;
      case 0xF20DFF00:
        frame.ir_data = "servo_right";
        break;
      case 0xFD00FF00:
      default:
        frame.ir_data = "stop";
        break;
    }

    IrReceiver.resume();
  }
  else
  {
    // timeout: cambia el estado interno
    if (lastIRTime != 0 && (millis() - lastIRTime > IR_TIMEOUT_MS))
    {
      frame.ir_data = "stop";
    }
  }
}
