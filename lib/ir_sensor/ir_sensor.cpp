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

void IrSensor::update(TelemetryState& state)
{
  if (IrReceiver.decode())
  {
    state.ir_new = true;
    state.ir_raw = IrReceiver.decodedIRData.decodedRawData;
    lastIRTime   = millis(); //

    switch (IrReceiver.decodedIRData.decodedRawData)
    {
      case 0xBC43FF00:
        status = IrSensorStatus::irSensorRight;
        break;
      case 0xBB44FF00:
        status = IrSensorStatus::irSensorLeft;
        break;
      case 0xB946FF00:
        status = IrSensorStatus::irSensorForward;
        break;
      case 0xEA15FF00:
        status = IrSensorStatus::irSensorBackward;
        break;
      case 0xE916FF00:
        status = IrSensorStatus::irSensorServoLeft;
        break;
      case 0xE619FF00:
        status = IrSensorStatus::irSensorServoCenter;
        break;
      case 0xF20DFF00:
        status = IrSensorStatus::irSensorServoRight;
        break;
      case 0xFD00FF00:
      default:
        status = IrSensorStatus::irSensorStop;
        break;
    }

    IrReceiver.resume();
  }
  else
  {
    // timeout: cambia el estado interno
    if (lastIRTime != 0 && (millis() - lastIRTime > IR_TIMEOUT_MS))
    {
      status = IrSensorStatus::irSensorStop;
    }
  }

  // una sola escritura al modelo común
  state.ir_mode = status;
}

const char* statusToString(IrSensorStatus status)
{
  switch (status)
  {
    case IrSensorStatus::irSensorForward:
      return "forward";
    case IrSensorStatus::irSensorBackward:
      return "backward";
    case IrSensorStatus::irSensorLeft:
      return "left";
    case IrSensorStatus::irSensorRight:
      return "right";
    case IrSensorStatus::irSensorStop:
      return "stop";
    case IrSensorStatus::irSensorServoLeft:
      return "servo_left";
    case IrSensorStatus::irSensorServoRight:
      return "servo_right";
    case IrSensorStatus::irSensorServoCenter:
      return "servo_center";
    case IrSensorStatus::Unknown:
    default:
      return "unknown";
  }
}
