// ===================== INCLUDES =====================
#include <Arduino.h>
#include <motor.h>
#include <ElegooSmartCar.h>
#include <Servo.h>
// #include <HCSR04.h>
// #include <SensorServo.h>
// #include <LineTracking.h>
// #include <Battery.h>
// #include <LEDRGB.h>  // Comentado para evitar conflictos con IRremote
// #include <Wire.h> //Sepuede eliminar
// #include <MPU6050.h>
#include "MPUControl.h"
// #include "MyCar.h"
#include "IrControl.h"

// ===================== DEFINICIÓN DE PINES Y OBJETOS =====================
void initializeModules();

Servo servo;
// HCSR04 sensor(TRIG_PIN, ECHO_PIN);
// MPU6050 mpu;
IRCONTROL irControl(IR_PIN);

MOTOR leftMotor(M_23_LEFT, LEFT_PWM, STBY);
MOTOR rightMotor(M_14_RIGHT, RIGHT_PWM, STBY);

// SENSORSERVO miSensorServo(sensor, servo);
// BATTERY battery(VOLTAGE_PIN);
// LINE_TRACKING lineTracker(LEFT_PIN, MIDDLE_PIN, RIGHT_PIN);
// LED_RGB led_rgb(RGB_PIN);

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

  // Control del modo manual
  irControl.setManualMode(true); // Activar modo manual (IR RECEIVING)
  // irControl.setManualMode(false); // Desactivar modo manual (IR IDLE)

  Serial.println("Sistema IR inicializado - listo para recibir comandos");
}

// // ===================== LOOP PRINCIPAL =====================
void loop()
{
  irControl.loop();
  // mpuControl.loop();
  // Si tienes un método tick en LED_RGB, llama aquí:
  // led_rgb.tick();
  // battery.getVoltage();
  // battery.printVoltage();

  // Control de motores basado en comandos IR
  if (irControl.getStatusCommand() == IR_MOVE_FORWARD)
  {
    leftMotor.forward(50);
    rightMotor.forward(50);
  }
  else if (irControl.getStatusCommand() == IR_MOVE_BACKWARD)
  {
    leftMotor.reverse(50);
    rightMotor.reverse(50);
  }
  else if (irControl.getStatusCommand() == IR_TURN_LEFT)
  {
    leftMotor.reverse(50);
    rightMotor.forward(50);
  }
  else if (irControl.getStatusCommand() == IR_TURN_RIGHT)
  {
    leftMotor.forward(50);
    rightMotor.reverse(50);
  }
  else if (irControl.getStatusCommand() == IR_STOP)
  {
    // leftMotor.freeStop();
    // rightMotor.freeStop()
    leftMotor.forceStop();
    rightMotor.forceStop();
  }
  else if (irControl.getStatusCommand() == IR_SERVO_LEFT)
  {
    servo.write(0);
  }
  else if (irControl.getStatusCommand() == IR_SERVO_CENTER)
  {
    servo.write(90);
  }
  else if (irControl.getStatusCommand() == IR_SERVO_RIGHT)
  {
    servo.write(180);
  }
}

// ===================== FUNCIONES AUXILIARES =====================
void initializeModules()
{
  // Inicializar sensor IR para lectura directa
  irControl.inizializeIR();
  Serial.println("Sensor IR inicializado para lectura de códigos");

  // 1. Inicializar I2C
  // Wire.begin();
  // delay(100); // Dar tiempo para que I2C se estabilice
  // mpu.initialize();
  // delay(100); // Esperar estabilización del sensor
  // 2. Inicializar el sensor MPU
  // mpuControl.initializeMPU();
  // 3. Inicializar el LED RGB
  // led_rgb.inizializeLEDRGB();

  // 4. Inicializar el servo
  pinMode(SERVO_PIN, OUTPUT);
  servo.attach(SERVO_PIN);
  delay(100);
  // mpu.initialize();
  // miCoche.begin(); // Primero inicializamos todos los componentes

  // 5. Inicializar el sensor IR
  // irControl.inizializeIR();
}
