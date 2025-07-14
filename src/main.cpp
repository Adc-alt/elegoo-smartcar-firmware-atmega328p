// ===================== INCLUDES =====================
#include <Arduino.h>
// #include <motor.h>
#include <ElegooSmartCar.h>
// #include <Servo.h>
// #include <HCSR04.h>
// #include <SensorServo.h>
// #include <LineTracking.h>
// #include <Battery.h>
#include <LEDRGB.h>
// #include <Wire.h>
// #include <MPU6050.h>
// #include "MPUControl.h"
// #include "MyCar.h"
// #include <IrControl.h>
// #include <IRremote.h> // Para pruebas directas de IR

// ===================== DEFINICIÓN DE PINES Y OBJETOS =====================
void initializeModules();

// Servo servo;
// HCSR04 sensor(TRIG_PIN, ECHO_PIN);
// MPU6050 mpu;
// IRCONTROL irControl(IR_PIN);

// MOTOR leftMotor(M_23_LEFT, LEFT_PWM, STBY);
// MOTOR rightMotor(M_14_RIGHT, RIGHT_PWM, STBY);

// SENSORSERVO miSensorServo(sensor, servo);
// BATTERY battery(VOLTAGE_PIN);
// LINE_TRACKING lineTracker(LEFT_PIN, MIDDLE_PIN, RIGHT_PIN);
LED_RGB led_rgb(RGB_PIN);

// MPUControl mpuControl(mpu);
// MyCar miCoche(miSensorServo, mpuControl, battery, lineTracker, led_rgb, leftMotor, rightMotor);

// // ===================== SETUP =====================

void setup()
{
  Serial.begin(USB_SPEED);
  initializeModules();
  
  // Dar tiempo al sensor HCSR04 para estabilizarse
  // delay(100);

  // miCoche.begin();
  // miSensorServo.startSearching();
}

// // ===================== LOOP PRINCIPAL =====================
void loop()
{

  // Si tienes un método tick en LED_RGB, llama aquí:
  // led_rgb.tick();
  // battery.getVoltage();
  // battery.printVoltage();
}

// ===================== FUNCIONES AUXILIARES =====================
void initializeModules()
{
  led_rgb.inizializeLEDRGB();

  // pinMode(SERVO_PIN, OUTPUT);
  // servo.attach(SERVO_PIN);
  // Wire.begin();
  // delay(1000);
  // mpu.initialize();
  // mpuControl.begin();
  // delay(100); // Dar tiempo para que I2C se estabilice
  // mpu.initialize();
  // miCoche.begin(); // Primero inicializamos todos los componentes
}
