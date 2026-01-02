#include "actuator_controller.h"

#include <string.h> // Para strcmp

ActuatorController::ActuatorController(CRGB* leds, size_t numLeds, Servo& servo, MOTOR& leftMotor, MOTOR& rightMotor)
    : leds(leds), numLeds(numLeds), servo(servo), leftMotor(leftMotor), rightMotor(rightMotor), previousServoAngle(90)
{
}

void ActuatorController::processCommands(const JsonDocument& receiveJson)
{
  // Procesar LED
  const char* ledColor = receiveJson["ledColor"] | "RED";
  processLed(ledColor);

  // Procesar Servo
  uint8_t angle = receiveJson["servoAngle"] | 90;
  processServo(angle);

  // Procesar Motores
  // Procesar Motores (objeto simple sin left/right)
  const char* motorAction = receiveJson["motors"]["action"] | "stop";
  uint8_t motorSpeed      = receiveJson["motors"]["speed"] | 0;
  processMotors(motorAction, motorSpeed);
}

void ActuatorController::processLed(const char* color)
{
  if (numLeds == 0 || leds == nullptr)
    return;

  switch (color[0])
  {
    case 'G': // GREEN
      leds[0] = CRGB::Green;
      break;
    case 'R': // RED
      leds[0] = CRGB::Red;
      break;
    case 'B': // BLUE
      leds[0] = CRGB::Blue;
      break;
    case 'P': // PURPLE
      leds[0] = CRGB::Purple;
      break;
    case 'Y': // YELLOW
      leds[0] = CRGB::Yellow;
      break;
    default:
      leds[0] = CRGB::Black;
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

MotorAction ActuatorController::parseMotorAction(const char* action)
{
  if (!action)
    return MotorAction::UNKNOWN;

  if (strcmp(action, "forward") == 0)
    return MotorAction::FORWARD;
  if (strcmp(action, "reverse") == 0)
    return MotorAction::REVERSE;
  if (strcmp(action, "turn_left") == 0)
    return MotorAction::TURN_LEFT;
  if (strcmp(action, "turn_right") == 0)
    return MotorAction::TURN_RIGHT;
  if (strcmp(action, "force_stop") == 0)
    return MotorAction::FORCE_STOP;
  if (strcmp(action, "free_stop") == 0)
    return MotorAction::FREE_STOP;

  return MotorAction::UNKNOWN;
}

void ActuatorController::processMotors(const char* action, uint8_t speed)
{
  // Convertir string a enum y usar switch-case (más limpio que múltiples if-else)
  MotorAction actionType = parseMotorAction(action);

  switch (actionType)
  {
    case MotorAction::FORWARD:
      leftMotor.forward(speed);
      rightMotor.forward(speed);
      break;

    case MotorAction::REVERSE:
      leftMotor.reverse(speed);
      rightMotor.reverse(speed);
      break;

    case MotorAction::TURN_LEFT:
      leftMotor.reverse(speed);
      rightMotor.forward(speed);
      break;

    case MotorAction::TURN_RIGHT:
      leftMotor.forward(speed);
      rightMotor.reverse(speed);
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
