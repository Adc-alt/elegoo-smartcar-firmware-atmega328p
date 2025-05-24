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
#include <IRremote.h>
#include <IrControl.h>

// ===================== DEFINICIÓN DE PINES Y OBJETOS =====================
void setupPins();

Servo servo;
HCSR04 sensor(TRIG_PIN, ECHO_PIN);

MOTOR leftMotor = MOTOR(M_23_LEFT, LEFT_PWM, STBY);
MOTOR rightMotor = MOTOR(M_14_RIGHT, RIGHT_PWM, STBY);

SENSORSERVO miSensorServo(sensor, servo);

BATTERY battery(VOLTAGE_PIN);
LINE_TRACKING lineTracker(LEFT_PIN, MIDDLE_PIN, RIGHT_PIN);
LED_RGB led_rgb(RGB_PIN);
IRCONTROL irControl(IR_PIN);

// ===================== SETUP =====================
void setup()
{
  Serial.begin(9600);
  irControl.begin();
}

void loop()
{
  irControl.loop();
  Serial.println(irControl.getStatus());
  delay(1000);
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
// void setupPins()
// {
//   pinMode(SERVO_PIN, OUTPUT);
//   servo.attach(SERVO_PIN);
// }