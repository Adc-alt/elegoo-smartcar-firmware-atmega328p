#include <Arduino.h>
#include <Servo.h>
#include <command_executor.h>
#include <command_frame.h>
#include <command_receiver.h>
#include <led_rgb.h>
#include <motor.h>

// Actuadores
MOTOR leftMotor(M_23_LEFT, LEFT_PWM, STBY);
MOTOR rightMotor(M_14_RIGHT, RIGHT_PWM, STBY);
LED_RGB led(RGB_PIN);
Servo servo;

// Comandos
CommandFrame commandFrame;
CommandReceiver commandReceiver(Serial);
CommandExecutor commandExecutor(leftMotor, rightMotor, servo, led);

void setup()
{
  Serial.begin(9600);
  leftMotor.forceStop();
  rightMotor.forceStop();
  servo.attach(SERVO_PIN);
  led.inizializeLEDRGB();
  Serial.println(F("[Main] Setup completado"));
}

void loop()
{
  // 1. Recibir comandos del ESP32
  if (commandReceiver.tryReceive(commandFrame))
  {
    // // Nuevo comando recibido
    // #ifdef DEBUG_COMMANDS
    //     Serial.print(F("[Main] Comando recibido en t="));
    //     Serial.print(commandFrame.lastUpdate_ms);
    //     Serial.println(F("ms"));
    // #endif
  }
  // 2. Ejecutar comandos en actuadores
  commandExecutor.execute(commandFrame);

  // 3. Resetear flags después de procesar
  commandFrame.hasNewCommand   = false;
  commandFrame.servoHasCommand = false;
  commandFrame.ledHasCommand   = false;
}