#include <Arduino.h>
// #include <motor.h>
#include <hardwarePins.h>
// #include <Servo.h>
#include <sensorServo.h>
// MOTOR leftMotor = MOTOR(M_23_LEFT, LEFT_PWM, STBY);
// MOTOR rightMotor = MOTOR(M_14_RIGHT, RIGHT_PWM, STBY);

SENSORSERVO miSensorServo(SERVO_PIN, TRIGGER_PIN, ECHO_PIN); // Pines: SERVO, TRIG, ECHO

SENSORSERVO_STATUS status = IDLE;
SENSORSERVO_STATUS prevStatus = IDLE;
// Servo miservolo;
void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  miSensorServo.init();
  Serial.println("Iniciando prueba del servo...");

  Serial.println((String) "Estado miSensorServo (setup): " + miSensorServo.getStatus());
  // miservolo.attach(10);
  miSensorServo.setAngle(90);
}

void loop()
{
  status = miSensorServo.getStatus();
  if (prevStatus != status)
  {
    prevStatus = status;
    Serial.println((String) "Estado miSensorServo (loop): " + miSensorServo.getStatus());
  }
  miSensorServo.loop();
  // miservolo.write(90);
  // put your main code here, to run repeatedly
  // leftMotor.forward(30);
  // leftMotor.forward(50);
  // rightMotor.forward(50);
  // delay(1000);
  // leftMotor.forceStop();
  // rightMotor.forceStop();
  // delay(2000);
}
