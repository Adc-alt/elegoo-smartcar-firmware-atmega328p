#include <Arduino.h>
#include <motor.h>
#include <ElegooSmartCar.h>
#include <Servo.h>
#include <HC-SR04.h>
#include <sensorServo.h>

void setupPins();

Servo servo;
HCSR04 sensor(ECHO_PIN, TRIGGER_PIN);

MOTOR leftMotor = MOTOR(M_23_LEFT, LEFT_PWM, STBY);
MOTOR rightMotor = MOTOR(M_14_RIGHT, RIGHT_PWM, STBY);

SENSORSERVO miSensorServo(sensor, servo);

void setup()
{
  Serial.begin(USB_SPEED);
  setupPins();

  miSensorServo.startSearching();
}

void loop()
{
}

void setupPins()
{
  pinMode(SERVO_PIN, OUTPUT);
  servo.attach(SERVO_PIN);
}
