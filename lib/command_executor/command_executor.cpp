#include "command_executor.h"

CommandExecutor::CommandExecutor(MOTOR& leftMotor, MOTOR& rightMotor, Servo& servo, LED_RGB& led)
    : leftMotor(leftMotor), rightMotor(rightMotor), servo(servo), led(led)
{
}

void CommandExecutor::execute(const CommandFrame& commandFrame)
{
  // Ejecutar cada tipo de comando
  executeMotors(commandFrame);
  executeServo(commandFrame);
  executeLed(commandFrame);
}

void CommandExecutor::executeMotors(const CommandFrame& commandFrame)
{
  // Motor izquierdo
  if (commandFrame.motors.leftAction != MotorAction::NONE)
  {
    switch (commandFrame.motors.leftAction)
    {
      case MotorAction::FORWARD:
        leftMotor.forward(commandFrame.motors.leftSpeed);
        break;
      case MotorAction::REVERSE:
        leftMotor.reverse(commandFrame.motors.leftSpeed);
        break;
      case MotorAction::FORCE_STOP:
        leftMotor.forceStop();
        break;
      case MotorAction::FREE_STOP:
        leftMotor.freeStop();
        break;
      case MotorAction::NONE:
        // No hacer nada
        break;
    }
  }

  // Motor derecho
  if (commandFrame.motors.rightAction != MotorAction::NONE)
  {
    switch (commandFrame.motors.rightAction)
    {
      case MotorAction::FORWARD:
        rightMotor.forward(commandFrame.motors.rightSpeed);
        break;
      case MotorAction::REVERSE:
        rightMotor.reverse(commandFrame.motors.rightSpeed);
        break;
      case MotorAction::FORCE_STOP:
        rightMotor.forceStop();
        break;
      case MotorAction::FREE_STOP:
        rightMotor.freeStop();
        break;
      case MotorAction::TURN_LEFT:
        rightMotor.turnLeft(commandFrame.motors.rightSpeed);
        break;
      case MotorAction::TURN_RIGHT:
        rightMotor.turnRight(commandFrame.motors.rightSpeed);
        break;
      case MotorAction::NONE:
        // No hacer nada
        break;
    }
  }
}

void CommandExecutor::executeServo(const CommandFrame& commandFrame)
{
  if (commandFrame.servoHasCommand)
  {
    servo.write(commandFrame.servoAngle);
    // Nota: El flag se resetea en el main después de procesar
  }
}

void CommandExecutor::executeLed(const CommandFrame& commandFrame)
{
  if (commandFrame.ledHasCommand)
  {
    switch (commandFrame.ledColor)
    {
      case LedColor::BLACK:
        led.setBlack();
        break;
      case LedColor::BLUE:
        led.setBlue();
        break;
      case LedColor::RED:
        led.setRed();
        break;
      case LedColor::YELLOW:
        led.setYellow();
        break;
      case LedColor::PURPLE:
        led.setPurple();
        break;
      case LedColor::GREEN:
        led.setGreen();
        break;
      case LedColor::CYAN:
        led.setCyan();
        break;
      case LedColor::GRAY:
        led.setGray();
        break;
      case LedColor::SALMON:
        led.setSalmon();
        break;
      case LedColor::NONE:
        // No hacer nada
        break;
    }
    // Nota: El flag se resetea en el main después de procesar
  }
}
