#include "actuator_controller.h"

#include <string.h> // Para strcmp

ActuatorController::ActuatorController(CRGB* leds, size_t numLeds, Servo& servo, MOTOR& leftMotor, MOTOR& rightMotor)
    : leds(leds), numLeds(numLeds), servo(servo), leftMotor(leftMotor), rightMotor(rightMotor), previousServoAngle(90)
{
  strncpy(previousLedColor, "BLACK", sizeof(previousLedColor) - 1);
  previousLedColor[sizeof(previousLedColor) - 1] = '\0';
}
// JSON compacto ESP32 → Atmega: sA=servoAngle, lC=ledColor, m=motors, L=left, R=right, a=action, s=speed
void ActuatorController::processCommands(const JsonDocument& receiveJson)
{
  if (receiveJson.containsKey("lC"))
  {
    const char* ledColor = receiveJson["lC"];
    if (ledColor != nullptr && strcmp(ledColor, previousLedColor) != 0)
    {
      strncpy(previousLedColor, ledColor, sizeof(previousLedColor) - 1);
      previousLedColor[sizeof(previousLedColor) - 1] = '\0';
      processLed(ledColor);
    }
  }

  if (receiveJson.containsKey("sA"))
  {
    uint8_t angle = receiveJson["sA"];
    processServo(angle);
  }

  if (receiveJson.containsKey("m"))
  {
    JsonObjectConst motors = receiveJson["m"].as<JsonObjectConst>();
    if (motors.containsKey("L") && motors.containsKey("R"))
    {
      const char* leftAction  = motors["L"]["a"];
      const char* rightAction = motors["R"]["a"];
      uint8_t leftSpeed      = motors["L"]["s"] | 0;
      uint8_t rightSpeed     = motors["R"]["s"] | 0;

      if (leftAction != nullptr)
        processDifferentialMotors(leftMotor, leftAction, leftSpeed, true);
      if (rightAction != nullptr)
        processDifferentialMotors(rightMotor, rightAction, rightSpeed, false);
    }
  }
}

// lC compacto: Y=YELLOW, B=BLUE, G=GREEN, P=PURPLE, W=WHITE, S=SALMON, C=CYAN; por defecto Y
void ActuatorController::processLed(const char* color)
{
  if (numLeds == 0 || leds == nullptr)
    return;

  if (color[0] == '\0')
    return;

  switch (color[0])
  {
    case 'Y':
      leds[0] = CRGB::Yellow;
      break;
    case 'B':
      leds[0] = CRGB::Blue;
      break;
    case 'G':
      leds[0] = CRGB::Green;
      break;
    case 'P':
      leds[0] = CRGB::Purple;
      break;
    case 'W':
      leds[0] = CRGB::White;
      break;
    case 'S':
      leds[0] = CRGB(255, 100, 50);
      break;
    case 'C':
      leds[0] = CRGB::Cyan;
      break;
    default:
      leds[0] = CRGB::Yellow; // por defecto Y
      break;
  }
  FastLED.show();
}

void ActuatorController::processServo(uint8_t angle)
{
  // Solo escribir si el ángulo cambió (optimización)
  if (angle != previousServoAngle)
  {
    servo.write(angle);
    previousServoAngle = angle;
  }
}

// Acciones compactas: fW=forward, bW=backward, tL=turnLeft, tR=turnRight, fS=freeStop, fT=forceStop
MotorAction ActuatorController::parseMotorAction(const char* action)
{
  if (!action)
    return MotorAction::UNKNOWN;

  if (strcmp(action, "fW") == 0)
    return MotorAction::FORWARD;
  if (strcmp(action, "bW") == 0)
    return MotorAction::BACKWARD;
  if (strcmp(action, "tL") == 0)
    return MotorAction::TURN_LEFT;
  if (strcmp(action, "tR") == 0)
    return MotorAction::TURN_RIGHT;
  if (strcmp(action, "fT") == 0)
    return MotorAction::FORCE_STOP;
  if (strcmp(action, "fS") == 0)
    return MotorAction::FREE_STOP;

  return MotorAction::UNKNOWN;
}

void ActuatorController::processDifferentialMotors(MOTOR& motor, const char* action, uint8_t speed, bool isLeftMotor)
{
  MotorAction actionType = parseMotorAction(action);

  switch (actionType)
  {
    case MotorAction::FORWARD:
      motor.forward(speed);
      break;
    case MotorAction::BACKWARD:
      motor.backward(speed);
      break;
    case MotorAction::FORCE_STOP:
      motor.forceStop();
      break;
    case MotorAction::FREE_STOP:
      motor.freeStop();
      break;
    case MotorAction::TURN_LEFT:
      // Giro izquierda: rueda izquierda atrás, rueda derecha adelante
      if (isLeftMotor)
        motor.backward(speed);
      else
        motor.forward(speed);
      break;
    case MotorAction::TURN_RIGHT:
      // Giro derecha: rueda izquierda adelante, rueda derecha atrás
      if (isLeftMotor)
        motor.forward(speed);
      else
        motor.backward(speed);
      break;
    case MotorAction::UNKNOWN:
    default:
      motor.freeStop(); // acción desconocida: soltar motores
      break;
  }
}

void ActuatorController::processMotors(const char* action, uint8_t speed)
{
  MotorAction actionType = parseMotorAction(action);

  switch (actionType)
  {
    case MotorAction::FORWARD:
      leftMotor.forward(speed);
      rightMotor.forward(speed);
      break;

    case MotorAction::BACKWARD:
      leftMotor.backward(speed);
      rightMotor.backward(speed);
      break;

    case MotorAction::TURN_LEFT:
      leftMotor.backward(speed);
      rightMotor.forward(speed);
      break;

    case MotorAction::TURN_RIGHT:
      leftMotor.forward(speed);
      rightMotor.backward(speed);
      break;

    case MotorAction::FORCE_STOP:
      leftMotor.forceStop();
      rightMotor.forceStop();
      break;

    case MotorAction::FREE_STOP:
    case MotorAction::UNKNOWN:
    default:
      leftMotor.freeStop();
      rightMotor.freeStop();
      break;
  }
}
