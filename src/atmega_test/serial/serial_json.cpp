#include <Arduino.h>
#include <ArduinoJson.h>
#include <FastLED.h>
#include <LED_RGB.h>
#include <battery.h>
#include <elegoo_smart_car_lib.h>
#include <hcsr04.h>
#include <ir_sensor.h>

// Objetos JSON
JsonDocument sendJson;
JsonDocument receiveJson;

// Variables de entrada para el JSON de envío (telemetría)
bool swPressed        = false;
int swCount           = 0;
int hcsr04DistanceCm  = 0;
int lineSensorLeft    = 0;
int lineSensorMiddle  = 0;
int lineSensorRight   = 0;
float batVoltage      = 0;
const char* irCommand = "stop";

// Variables para el JSON de recepción (comandos)
int servoAnglePrevious = 23;
int servoAngle         = 23;
// String ledColor        = "RED";

// Variable para control de tiempo de envío
unsigned long lastSendTime        = 0;
const unsigned long SEND_INTERVAL = 500; // 500ms

// Variables para control de timeout de recepción
unsigned long lastReceiveTime        = 0;
bool timeoutActive                   = false;
const unsigned long TIMEOUT_INTERVAL = 2000; // 2 segundos

// Variable referencia
bool validHcsr04 = false;

// Instancias sensores
Hcsr04 hcsr04(TRIG_PIN, ECHO_PIN);
IrSensor irSensor(IR_PIN);
Battery batterySensor(BATTERY_VOLTAGE_PIN);

// Instancias LED
CRGB leds[NUM_LEDS];

// Declaraciones de funciones
void setupPins();
void readInput();
void initializeJsons();
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

  initializeJsons();

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
  checkTimeout();

  // 2. ACTUALIZAR ESTADOS/ COMANDOS
  processCommands();

  // 3. ESCRIBIR SALIDAS
  //  Comprobar si hay que enviar (cada 500ms)
  unsigned long currentTime = millis();
  if (currentTime - lastSendTime >= SEND_INTERVAL)
  {
    sendJsonBySerial();
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
}

void readInput()
{
  // Actualizar las variables de entrada de los sensores
  // Leer el botón (en pull-up, LOW significa presionado)
  swPressed        = (!digitalRead(SWITCH_PIN));
  hcsr04DistanceCm = hcsr04.getDistanceCm(validHcsr04);
  lineSensorLeft   = analogRead(LINE_LEFT_PIN);
  lineSensorMiddle = analogRead(LINE_MIDDLE_PIN);
  lineSensorRight  = analogRead(LINE_RIGHT_PIN);
  batVoltage       = batterySensor.getVoltage();
  irCommand        = irSensor.getIrCommand();
}

void initializeJsons()
{
  // Inicializar el objeto JSON de envío
  sendJson["swPressed"]        = false;
  sendJson["swCount"]          = 0;
  sendJson["hcsr04DistanceCm"] = 0;
  sendJson["lineSensorLeft"]   = 0;
  sendJson["lineSensorMiddle"] = 0;
  sendJson["lineSensorRight"]  = 0;
  sendJson["irCommand"]        = "stop";
  sendJson["batVoltage"]       = 0;

  // Inicializar el objeto JSON de recepción
  receiveJson["servoAngle"] = 90;
  receiveJson["ledColor"]   = "RED";
}

void sendJsonBySerial()
{
  // Actualizar sendJson a partir de las variables de entrada
  sendJson["swPressed"]        = swPressed;
  sendJson["swCount"]          = servoAngle;
  sendJson["hcsr04DistanceCm"] = hcsr04DistanceCm;
  sendJson["lineSensorLeft"]   = lineSensorLeft;
  sendJson["lineSensorMiddle"] = lineSensorMiddle;
  sendJson["lineSensorRight"]  = lineSensorRight;
  sendJson["irCommand"]        = irCommand;
  sendJson["batVoltage"]       = batVoltage;

  // Enviar por serial
  serializeJson(sendJson, Serial);
  Serial.write('\n');
}

void readJsonBySerial()
{
  // Leer el JSON recibido
  String jsonString = Serial.readStringUntil('\n');
  jsonString.trim();

  // Deserializar el JSON recibido
  DeserializationError error = deserializeJson(receiveJson, jsonString);

  if (!error)
  {
    // Actualizar las variables a partir del receiveJson
    servoAngle = receiveJson["servoAngle"];
    // ledColor   = receiveJson["ledColor"].as<String>();

    // Actualizar el tiempo de última recepción exitosa
    lastReceiveTime = millis();
    timeoutActive   = false;
  }
}

void checkTimeout()
{
  unsigned long currentTime = millis();

  // Verificar si ha pasado más de 2 segundos desde la última recepción
  if (lastReceiveTime > 0 && (currentTime - lastReceiveTime >= TIMEOUT_INTERVAL && !timeoutActive))
  {
    timeoutActive = true;
    Serial.println(F("Timeout de recepción"));
  }
  else if (lastReceiveTime == 0)
  {
    // Si nunca se ha recibido nada, inicializar el tiempo
    lastReceiveTime = currentTime;
  }
}

void processCommands()
{
  if (receiveJson["ledColor"].as<String>() == "GREEN")
  {
    leds[0] = CRGB::Green;
    FastLED.show();
  }
}