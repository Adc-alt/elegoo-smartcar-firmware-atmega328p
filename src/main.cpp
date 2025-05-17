#include <Arduino.h>
// // #include <motor.h>
#include <ElegooSmartCar.h>
// // #include <Servo.h>
// // #include <sensorServo.h>
// #include <motor.h>
#include <HC-SR04.h>
#include <LINE_TRACKING.h>

// MOTOR leftMotor = MOTOR(M_23_LEFT, LEFT_PWM, STBY);
// HCSR04 ultrasonic = HCSR04(TRIG_PIN, ECHO_PIN); // Usa los pines que correspondan a tu conexión
// IR_Sensor irSensor(9);                          // Pin 9 para el sensor infrarrojo

// #define VOLTAGE_PIN A3
// unsigned long lastMeasureTime = 0;
// #include <FastLED.h>

// Crear instancia del sensor HC-SR04
// HCSR04 ultrasonic(TRIG_PIN, ECHO_PIN); // TRIG en pin 13, ECHO en pin 12

// MOTOR leftMotor = MOTOR(M_23_LEFT, LEFT_PWM, STBY);
// LINE_TRACKING lineTracking(LEFT_PIN,MIDDLE_PIN,RIGHT_PIN);

// void setup()
// {
//   Serial.begin(9600);
//   Serial.println("Iniciando sensor HC-SR04...");
//   pinMode(LEFT_PIN, INPUT);
//   pinMode(MIDDLE_PIN, INPUT);
//   pinMode(RIGHT_PIN, INPUT);
// }

// void loop()
// {
//   Serial.println("Lectura de pines:");
//   Serial.print("Pin LEFT: ");
//   Serial.println(analogRead(LEFT_PIN));
//   Serial.print("Pin MIDDLE: ");
//   Serial.println(analogRead(MIDDLE_PIN));
//   Serial.print("Pin RIGHT: ");
//   Serial.println(analogRead(RIGHT_PIN));
//   Serial.println("---");
//   delay(1000);
// }


LINE_TRACKING lineTracking(LEFT_PIN,MIDDLE_PIN,RIGHT_PIN);

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  lineTracking.tick();
  Serial.println((String)"status: " + lineTracking.getstatus());
  delay(1000);
}

