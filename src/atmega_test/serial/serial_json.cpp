#include <Arduino.h>
#include <ArduinoJson.h>
#include <FastLED.h>
#include <LED_RGB.h>
#include <MPU6050.h>
#include <Servo.h>
#include <Wire.h>
#include <actuator_controller.h>
#include <battery.h>
#include <elegoo_smart_car_lib.h>
#include <hcsr04.h>
#include <ir_sensor.h>
#include <mpu.h>

// Buffer estático para recibir JSON (reducido: JSON recibido es pequeño ~37 chars)
char jsonBuffer[128];

// Documento JSON estático reutilizable (memoria fija, nada de heap)
// Tamaño aumentado a 256 bytes para soportar objetos anidados (motors con action y speed)
// Se reutiliza con clear() antes de cada uso (deserialización o construcción)
static StaticJsonDocument<256> jsonDoc;

// Variables de entrada para el JSON de envío (telemetría)
bool swPressed            = false;
uint16_t swCount          = 0; // Cambiado de int (2 bytes) a uint16_t (2 bytes, más claro)
uint8_t hcsr04DistanceCm  = 0;
uint16_t lineSensorLeft   = 0; // Cambiado de int a uint16_t (mismo tamaño, más explícito)
uint16_t lineSensorMiddle = 0;
uint16_t lineSensorRight  = 0;
uint16_t batVoltage       = 0; // Multiplicado por 100 (ej: 3.70V → 370)
int16_t mpuAccelX         = 0; // Multiplicado por 100 (ej: -0.12 → -12)
int16_t mpuAccelY         = 0;
int16_t mpuAccelZ         = 0;
int16_t mpuGyroX          = 0; // Multiplicado por 100 (ej: 1.23 → 123)
int16_t mpuGyroY          = 0;
int16_t mpuGyroZ          = 0;
const char* irCommand     = "stop";

// Variables para el JSON de recepción (comandos)
uint8_t servoAnglePrevious = 23; // 0-200 grados → uint8_t suficiente
uint8_t servoAngle         = 23; // 0-200 grados → uint8_t suficiente
// String ledColor        = "RED";

// Flag para indicar si hay un nuevo JSON válido para procesar
bool hasNewJson = false;

// Variable para control de tiempo de envío
unsigned long lastSendTime        = 0;
const unsigned long SEND_INTERVAL = 500; // 500ms

// Variables para control de timeout de recepción
unsigned long lastReceiveTime        = 0;
bool timeoutActive                   = false;
const unsigned long TIMEOUT_INTERVAL = 2000; // 2 segundos

// Variable referencia
bool validHcsr04 = false;

// Instancias Sensores
Hcsr04 hcsr04(TRIG_PIN, ECHO_PIN);
IrSensor irSensor(IR_PIN);
Battery batterySensor(BATTERY_VOLTAGE_PIN);
MPU6050 mpu(0x68);
Mpu mpuSensor(mpu);
Servo servo;

// Instancias Actuadores
CRGB leds[NUM_LEDS];
MOTOR leftMotor(M_23_LEFT, LEFT_PWM, STBY);
MOTOR rightMotor(M_14_RIGHT, RIGHT_PWM, STBY);
ActuatorController actuatorController(leds, NUM_LEDS, servo, leftMotor, rightMotor);

// Declaraciones de funciones
void setupPins();
void readInput();
void sendJsonBySerial();
void readJsonBySerial();
void checkTimeout();
void processCommands();

void setup()
{
  Serial.begin(9600);

  setupPins();
  hcsr04.begin();
  irSensor.begin();
  batterySensor.begin();

  // Inicializar MPU6050 correctamente
  Wire.begin();      // Configura el hardware I2C
  delay(100);        // Tiempo para que I2C se estabilice
  mpu.initialize();  // Inicializa el sensor MPU6050
  delay(100);        // Tiempo para que el sensor se estabilice
  mpuSensor.begin(); // Calibra el sensor

  delay(500);
}

void loop()
{
  // 1. LEER ENTRADAS
  //  Leer entradas de los sensores
  readInput();

  // RECEPCIÓN JSON DE COMANDOS
  //  Comprobar si hay datos disponibles en serial
  if (Serial.available() > 0)
  {
    readJsonBySerial();
  }
  // CONTROL DE TIMEOUT DE RECEPCIÓN
  //  Verificar timeout de recepción
  // TODO:
  // Implementar timeout solo si se recibe JSON

  checkTimeout();

  // 2. ACTUALIZAR ESTADOS/ COMANDOS
  processCommands();

  // 3. ESCRIBIR SALIDAS
  //  Comprobar si hay que enviar (cada 500ms)
  unsigned long currentTime = millis();
  if (currentTime - lastSendTime >= SEND_INTERVAL)
  {
    // sendJsonBySerial();
    lastSendTime = currentTime;
    swCount++;
  }
}

// Implementaciones de funciones
void setupPins()
{
  // Inicializar pin 3 como botón con pull-up interno
  pinMode(SWITCH_PIN, INPUT_PULLUP);
  pinMode(LINE_LEFT_PIN, INPUT);
  pinMode(LINE_MIDDLE_PIN, INPUT);
  pinMode(LINE_RIGHT_PIN, INPUT);
  FastLED.addLeds<WS2812, RGB_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  servo.attach(SERVO_PIN);
}

void readInput()
{
  // Actualizar las variables de entrada de los sensores
  // Leer el botón (en pull-up, LOW significa presionado)
  swPressed        = (!digitalRead(SWITCH_PIN));
  hcsr04DistanceCm = (uint8_t)hcsr04.getDistanceCm(validHcsr04); // Cast a uint8_t (0-50 cm)
  lineSensorLeft   = analogRead(LINE_LEFT_PIN);
  lineSensorMiddle = analogRead(LINE_MIDDLE_PIN);
  lineSensorRight  = analogRead(LINE_RIGHT_PIN);
  batVoltage       = (uint16_t)(batterySensor.getVoltage() * 100); // Multiplicar por 100 (ej: 3.70V → 370)

  // mpu
  mpuSensor.getMpuData();
  mpuAccelX = (int16_t)(mpuSensor.getValue(Mpu::ACCEL_X) * 100);
  mpuAccelY = (int16_t)(mpuSensor.getValue(Mpu::ACCEL_Y) * 100);
  mpuAccelZ = (int16_t)(mpuSensor.getValue(Mpu::ACCEL_Z) * 100);
  mpuGyroX  = (int16_t)(mpuSensor.getValue(Mpu::GYRO_X) * 100);
  mpuGyroY  = (int16_t)(mpuSensor.getValue(Mpu::GYRO_Y) * 100);
  mpuGyroZ  = (int16_t)(mpuSensor.getValue(Mpu::GYRO_Z) * 100);
  irCommand = irSensor.getIrCommand();
}

void sendJsonBySerial()
{
  // Limpiar el documento reutilizable antes de construir la telemetría
  jsonDoc.clear();

  // Construir JSON de telemetría a partir de las variables de entrada
  jsonDoc["swPressed"]        = swPressed;
  jsonDoc["swCount"]          = swCount;
  jsonDoc["hcsr04DistanceCm"] = hcsr04DistanceCm;
  jsonDoc["lineSensorLeft"]   = lineSensorLeft;
  jsonDoc["lineSensorMiddle"] = lineSensorMiddle;
  jsonDoc["lineSensorRight"]  = lineSensorRight;
  jsonDoc["irCommand"]        = irCommand;
  jsonDoc["batVoltage"]       = batVoltage;
  jsonDoc["mpuAccelX"]        = mpuAccelX;
  jsonDoc["mpuAccelY"]        = mpuAccelY;
  jsonDoc["mpuAccelZ"]        = mpuAccelZ;
  jsonDoc["mpuGyroX"]         = mpuGyroX;
  jsonDoc["mpuGyroY"]         = mpuGyroY;
  jsonDoc["mpuGyroZ"]         = mpuGyroZ;

  // Enviar por serial
  serializeJson(jsonDoc, Serial);
  Serial.write('\n');
}

void readJsonBySerial()
{
  // Lee una línea a buffer fijo (sin String)
  size_t n      = Serial.readBytesUntil('\n', jsonBuffer, sizeof(jsonBuffer) - 1);
  jsonBuffer[n] = '\0';

  // Limpiar el documento reutilizable antes de cada uso
  jsonDoc.clear();

  // Deserializar sin filter (JSON recibido es pequeño, no es necesario)
  DeserializationError error = deserializeJson(jsonDoc, jsonBuffer);

  if (!error)
  {
    servoAngle = jsonDoc["servoAngle"] | servoAngle;

    hasNewJson      = true;
    lastReceiveTime = millis();
    timeoutActive   = false;
  }
  else
  {
    hasNewJson = false;
  }
}

void checkTimeout()
{
  unsigned long currentTime = millis();

  if (lastReceiveTime > 0 && (currentTime - lastReceiveTime >= TIMEOUT_INTERVAL && !timeoutActive))
  {
    timeoutActive = true;
  }
  else if (lastReceiveTime == 0)
  {
    lastReceiveTime = currentTime;
  }
}

void processCommands()
{
  if (hasNewJson && jsonDoc.size() > 0)
  {
    actuatorController.processCommands(jsonDoc);
    hasNewJson = false;
  }
}