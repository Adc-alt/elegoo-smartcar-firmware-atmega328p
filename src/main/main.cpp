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

// Buffer para recibir JSON: debe caber el mensaje completo para no truncar
// Ejemplo compacto ESP32→ATmega (claves cortas, ver ActuatorController):
// {"sA":90,"lC":"Y","Md":4,"m":{"L":{"a":"fS","s":0},"R":{"a":"fS","s":0}}}
// lC es un código de un carácter (p. ej. "Y" amarillo); no depende del nombre del enum LED_RGB (YELLOW/YELOW).
char jsonBuffer[128];
size_t jsonBufferIndex = 0;     // Índice para acumular bytes en el buffer
bool inFrame           = false; // Flag para sincronización: esperamos '{' antes de acumular

// Documento JSON estático reutilizable (memoria fija, nada de heap)
// Tamaño acorde al buffer de recepción para objetos anidados (motors left/right)
// Se reutiliza con clear() antes de cada uso (deserialización o construcción)
static StaticJsonDocument<128> jsonDoc;

// Variables de entrada para el JSON de envío (telemetría)
bool swPressed           = false;
uint16_t swCount         = 0; // Contador de envíos de telemetría (también expuesto en JSON)
uint8_t hcsr04DistanceCm = 0;
int lineSensorLeft       = 0; // analogRead(): 0-1023
int lineSensorMiddle     = 0;
int lineSensorRight      = 0;
uint16_t batVoltage      = 0; // Multiplicado por 100 (ej: 3.70V → 370)
int16_t mpuAccelX        = 0; // Multiplicado por 100 (ej: -0.12 → -12)
int16_t mpuAccelY        = 0;
int16_t mpuAccelZ        = 0;
int16_t mpuGyroX         = 0; // Multiplicado por 100 (ej: 1.23 → 123)
int16_t mpuGyroY         = 0;
int16_t mpuGyroZ         = 0;
uint32_t irRaw           = 0; // Valor raw hexadecimal del IR (0xBC43FF00, etc.)

// Variables para el JSON de recepción (comandos)
uint8_t servoAngle = 23; // 0-200 grados → uint8_t suficiente
// Md: 0=IR, 1=OBSTACLE_AVOIDANCE, 2=FOLLOW, 3=LINE_FOLLOWING, 4=RC, 5=BALL_FOLLOW, 6=IDLE
uint8_t currentMode = 6; // por defecto IDLE (no leer HCSR04)

// Flag para indicar si hay un nuevo JSON válido para procesar
bool hasNewJson = false;

// Variable para control de tiempo de envío
unsigned long lastSendTime        = 0;
const unsigned long SEND_INTERVAL = 100; // ms entre envíos de telemetría por serial

// Variables para control de timeout de recepción
unsigned long lastReceiveTime        = 0;
bool timeoutActive                   = false;
const unsigned long TIMEOUT_INTERVAL = 2000; // 2 segundos

// Última lectura HCSR04 válida (p. ej. sin timeout en pulseIn)
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
  // Inicializar comunicación serial
  Serial.begin(115200);

  // Inicializar pines
  setupPins();
  // Inicializar sensores
  hcsr04.begin();
  irSensor.begin();
  delay(50);
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
  // 1. LEER ENTRADAS RECEPCION COMANDOS JSON [5.6ms]
  readJsonBySerial();
  checkTimeout();

  // 2. ACTUALIZAR ESTADOS/ COMANDOS [4.3ms]
  // unsigned long processCommandsStartTime = micros(); // Medir tiempo de processCommands()
  processCommands();
  // unsigned long processCommandsDuration = micros() - processCommandsStartTime; // Tiempo que tarda processCommands()

  // Imprimir tiempo de processCommands()
  // if (processCommandsDuration > 100)
  // {
  //   Serial.print(F("[processCommands] Tiempo: "));
  //   Serial.print(processCommandsDuration);
  //   Serial.print(F(" us\n"));
  // }

  // 3 LEER ENTRADAS DE SENSORES (puede tardar hasta 25ms con HCSR04)
  // unsigned long sensorStartTime = micros(); // Medir tiempo de readInput()
  readInput();
  // unsigned long sensorReadTime = micros() - sensorStartTime; // Tiempo que tarda readInput()

  // Imprimir tiempo de readInput()
  // if (sensorReadTime > 100)
  // {
  // Serial.print(F("[readInput] Tiempo: "));
  // Serial.print(sensorReadTime);
  // Serial.print(F(" us\n"));
  // }

  // RECEPCIÓN JSON DE COMANDOS
  //  Leer datos disponibles en serial (no bloqueante)
  // CONTROL DE TIMEOUT DE RECEPCIÓN
  //  Verificar timeout de recepción

  // 3.5 ESCRIBIR SALIDAS[20ms] sendjson[15ms]
  //  Comprobar si hay que enviar (cada 500ms)
  unsigned long currentTime = millis();
  if (currentTime - lastSendTime >= SEND_INTERVAL)
  {
    // unsigned long telemetryStartTime = micros(); // Medir tiempo de envío de telemetría
    sendJsonBySerial();
    // unsigned long telemetryDuration = micros() - telemetryStartTime; // Tiempo que tarda sendJsonBySerial()

    // Serial.print(F("[sendJson] Tiempo: "));
    // Serial.print(telemetryDuration);
    // Serial.print(F(" us\n"));

    lastSendTime = currentTime;
    swCount++;
  }
}

// Implementaciones de funciones
void setupPins()
{
  // Inicializar pin 3 como botón con pull-up interno
  pinMode(SWITCH_PIN, INPUT_PULLUP);
  FastLED.addLeds<WS2812, RGB_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  servo.attach(SERVO_PIN);
}

void readInput()
{
  // Actualizar las variables de entrada de los sensores
  swPressed = (!digitalRead(SWITCH_PIN));

  // HCSR04 solo en modo 1 (OBSTACLE_AVOIDANCE) o 2 (FOLLOW) para ahorrar ~25 ms por ciclo
  if (currentMode == 1 || currentMode == 2)
    hcsr04DistanceCm = (uint8_t)hcsr04.getDistanceCm(validHcsr04);

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
  // Solo actualizar irRaw cuando hay un comando nuevo (getIrRaw() != 0)
  uint32_t newIrRaw = irSensor.getIrRaw();
  if (newIrRaw != 0)
  {
    irRaw = newIrRaw;
  }
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
  jsonDoc["irRaw"]            = irRaw; // Valor raw hexadecimal (0xBC43FF00, etc.)
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

  // Resetear irRaw después de enviarlo para que solo se envíe una vez
  irRaw = 0;
}

void readJsonBySerial()
{
  while (Serial.available() > 0)
  {
    char c = Serial.read();

    // 1) Esperar al inicio de frame: '{'
    if (!inFrame)
    {
      if (c == '{')
      {
        inFrame                       = true;
        jsonBufferIndex               = 0;
        jsonBuffer[jsonBufferIndex++] = c;
      }
      // si no es '{', ignoramos todo
      continue;
    }

    // 2) Ya estamos dentro: acumular hasta '\n'
    if (c == '\n')
    {
      // fin de mensaje
      jsonBuffer[jsonBufferIndex] = '\0';
      inFrame                     = false;

#ifdef DEBUG
      Serial.print(F(">>> JSON recibido: "));
      Serial.println(jsonBuffer);
#endif

      // Limpiar el documento reutilizable antes de cada uso
      jsonDoc.clear();

      // Deserializar sin filter (JSON recibido es pequeño, no es necesario)
      DeserializationError error = deserializeJson(jsonDoc, jsonBuffer);

      if (!error)
      {
        servoAngle  = jsonDoc["sA"] | servoAngle;
        currentMode = jsonDoc["Md"] | 6u; // 6 = IDLE si no viene Md

        hasNewJson      = true;
        lastReceiveTime = millis();
        timeoutActive   = false;
      }
      else
      {
        // Serial.print(F(">>> Error deserializando JSON: "));
        // Serial.println(error.c_str());
        hasNewJson = false;
      }

      jsonBufferIndex = 0;
      continue;
    }

    // Ignorar '\r'
    if (c == '\r')
      continue;

    // 3) Meter byte en buffer si hay espacio
    if (jsonBufferIndex < sizeof(jsonBuffer) - 1)
    {
      jsonBuffer[jsonBufferIndex++] = c;
    }
    else
    {
      // OVERFLOW: mensaje demasiado largo o corrupto
      // => abortar frame y re-sincronizar
      // Serial.println(F(">>> Buffer overflow, re-sincronizando..."));
      jsonBufferIndex = 0;
      inFrame         = false;
      // A partir de aquí, ignoramos hasta ver el próximo '{'
    }
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