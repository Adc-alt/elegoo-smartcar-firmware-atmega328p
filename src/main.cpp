// #include <Arduino.h>
#include <motor.h>
#include <hardwarePins.h>

MOTOR leftMotor =  MOTOR(M_23_LEFT,LEFT_PWM,STBY);
MOTOR rightMotor = MOTOR(M_14_RIGHT,RIGHT_PWM,STBY);


void setup() 
{
  // put your setup code here, to run once:
  
}

void loop() 
{
  // put your main code here, to run repeatedly
  // leftMotor.forward(30);
  leftMotor.forward(50);
  rightMotor.forward(50);
  delay(1000);
  leftMotor.forceStop();
  rightMotor.forceStop();
  delay(2000);
}

