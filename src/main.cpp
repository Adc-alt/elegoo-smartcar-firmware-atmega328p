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
bool bandera = true;
Servo miservolo;
void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  miSensorServo.init();
  Serial.println("Iniciando prueba del servo...");

  Serial.println((String) "Estado miSensorServo (setup): " + miSensorServo.getStatus());
  // miservolo.attach(10);
  // miSensorServo.setAngle(90);
  // Serial.println((String) "Estado miSensorServo (setup): " + miSensorServo.getStatus());
}

void loop()
{
  miSensorServo.loop();

  // Solo actualizamos el estado cuando cambia
  status = miSensorServo.getStatus();
  if (prevStatus != status)
  {
    prevStatus = status;
    Serial.println((String) "Estado miSensorServo: " + status);
  }

  // Solo medimos la distancia cuando estamos en IDLE
  if (status == IDLE)
  {
    uint8_t distance = miSensorServo.getDistance();
    Serial.println((String) "Distancia: " + distance + " cm");
  }
}

// void setup()
// {
//   Serial.begin(9600);
//   pinMode(LEFT_PIN, INPUT); // Changed to INPUT for analogRead
//   pinMode(MIDDLE_PIN, INPUT);
//   pinMode(RIGHT_PIN, INPUT);
// }

// void loop()
// {
//   int ls = analogRead(LEFT_PIN);                                        // Declared ls as int
//   Serial.println((String) "La lectura del sensor izquierdo es: " + ls); // Added semicolon
//   int lm = analogRead(MIDDLE_PIN);                                        // Declared ls as int
//   Serial.println((String) "La lectura del sensor centro es: " + lm); // Added semicolon
//   int lr = analogRead(RIGHT_PIN);                                        // Declared ls as int
//   Serial.println((String) "La lectura del sensor derecho es: " + lr); // Added semicolon

// }