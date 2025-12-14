#include "line_sensor.h"

// 1. Constructor
LineSensor::LineSensor(uint8_t pinLeft, uint8_t pinMiddle, uint8_t pinRight)
{
  this->pinLeft   = pinLeft;
  this->pinMiddle = pinMiddle;
  this->pinRight  = pinRight;
}

void LineSensor::begin()
{
  pinMode(this->pinLeft, INPUT);
  pinMode(this->pinMiddle, INPUT);
  pinMode(this->pinRight, INPUT);
}

void LineSensor::update(TelemetryFrame& frame)
{
  frame.line_sensor_left   = analogRead(this->pinLeft);
  frame.line_sensor_middle = analogRead(this->pinMiddle);
  frame.line_sensor_right  = analogRead(this->pinRight);
}
