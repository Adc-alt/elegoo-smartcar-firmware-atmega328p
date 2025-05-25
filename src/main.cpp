// ===================== INCLUDES =====================
#include <Arduino.h>
#include <motor.h>
#include <ElegooSmartCar.h>
#include <Servo.h>
#include <HCSR04.h>
#include <SensorServo.h>
#include <LineTracking.h>
#include <Battery.h>
#include <LEDRGB.h>
#include <Wire.h>
#include <MPU6050.h>
#include "MPUControl.h"
#include "MyCar.h"

// ===================== DEFINICIÓN DE PINES Y OBJETOS =====================
void setupPins();

Servo servo;
HCSR04 sensor(TRIG_PIN, ECHO_PIN);
MPU6050 mpu;

MOTOR leftMotor = MOTOR(M_23_LEFT, LEFT_PWM, STBY);
MOTOR rightMotor = MOTOR(M_14_RIGHT, RIGHT_PWM, STBY);

SENSORSERVO miSensorServo(sensor, servo);
BATTERY battery(VOLTAGE_PIN);
LINE_TRACKING lineTracker(LEFT_PIN, MIDDLE_PIN, RIGHT_PIN);
LED_RGB led_rgb(RGB_PIN);

MPUControl mpuControl(mpu);
MyCar miCoche(miSensorServo, mpuControl, battery, lineTracker, led_rgb, leftMotor);

// ===================== SETUP =====================
void calibrateMPU();

void setup()
{
  Serial.begin(USB_SPEED);
  setupPins();
  miCoche.begin();
}

void loop()
{
  miCoche.loop();
  delay(100); // Pequeño delay para no saturar el puerto serie
}

// void setup()
// {
//   Serial.begin(USB_SPEED);
//   irrecv.enableIRIn(); // Inicia el receptor IR
// }

// // ===================== LOOP PRINCIPAL =====================
// void loop()
// {

//   // Si tienes un método tick en LED_RGB, llama aquí:
//   // led_rgb.tick();

// }

// ===================== FUNCIONES AUXILIARES =====================
void setupPins()
{
  pinMode(SERVO_PIN, OUTPUT);
  servo.attach(SERVO_PIN);
  Wire.begin();
  delay(100); // Dar tiempo para que I2C se estabilice
  mpu.initialize();
}
