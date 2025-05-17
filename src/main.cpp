#include <Arduino.h>
#include <motor.h>
#include <ElegooSmartCar.h>
#include <Servo.h>
#include <HC-SR04.h>
#include <sensorServo.h>

void setupPins();

Servo servo;
HCSR04 sensor(TRIG_PIN, ECHO_PIN);

MOTOR leftMotor = MOTOR(M_23_LEFT, LEFT_PWM, STBY);
MOTOR rightMotor = MOTOR(M_14_RIGHT, RIGHT_PWM, STBY);

SENSORSERVO miSensorServo(sensor, servo);

void setup()
{
  Serial.begin(USB_SPEED);

  Serial.println("TEST SENSOR SERVO");
  delay(2000);
  setupPins();

  miSensorServo.startSearching();
}

unsigned long printTime = 0;
void loop()
{
  miSensorServo.loop();
  if ((millis() - printTime) > 2000)
  {
    printTime = millis();
    Serial.println((String) "searchAngle: " + miSensorServo.getSearchAngle());
  }
}

void setupPins()
{
  pinMode(SERVO_PIN, OUTPUT);
  servo.attach(SERVO_PIN);
}
