#include <Arduino.h>
#include <ArduinoJson.h>
#include <elegoo_smart_car_lib.h>

// Objetos JSON
JsonDocument sendJson;
JsonDocument receiveJson;

// Variables de entrada para el JSON de envío (telemetría)

bool swPressed       = false;
int swCount          = 0;
int hcsr04DistanceCm = 0;

// Variables para el JSON de recepción (comandos)
int servoAnglePrevious = 23;
int servoAngle         = 23;
String ledColor        = "RED";

// Variable para control de tiempo de envío
unsigned long lastSendTime        = 0;
const unsigned long SEND_INTERVAL = 500; // 500ms

// Variables para control de timeout de recepción
unsigned long lastReceiveTime        = 0;
bool timeoutActive                   = false;
const unsigned long TIMEOUT_INTERVAL = 2000; // 2 segundos

// Definiciones de pines
const int BUTTON_PIN = 3;

// Declaraciones de funciones
void setupPins();
void readInput();
void initializeJsons();
void sendJsonBySerial();
void readJsonBySerial();
void checkTimeout();

void setup()
{
  Serial.begin(9600);
  setupPins();
  initializeJsons();
}

void loop()
{
  // Leer entradas de los sensores
  readInput();

  // Comprobar si hay que enviar (cada 500ms)
  unsigned long currentTime = millis();
  if (currentTime - lastSendTime >= SEND_INTERVAL)
  {
    sendJsonBySerial();
    lastSendTime = currentTime;
    swCount++;
    // Serial.println("Servo angle sent: " + String(servoAngle));
  }

  // Comprobar si hay datos disponibles en serial
  if (Serial.available() > 0)
  {
    readJsonBySerial();
  }
  if (servoAngle != servoAnglePrevious)
  {
    servoAnglePrevious = servoAngle;
    // Serial.println("servoAngle: " + String(servoAngle));
  }

  // Verificar timeout de recepción
  checkTimeout();
}

// Implementaciones de funciones
void setupPins()
{
  // Inicializar pin 3 como botón con pull-up interno
  pinMode(SWITCH_PIN, INPUT_PULLUP);
}

void readInput()
{
  // Actualizar las variables de entrada de los sensores
  // Leer el botón (en pull-up, LOW significa presionado)
  swPressed = (!digitalRead(SWITCH_PIN));

  // Aquí puedes leer otros sensores cuando los conectes
  // Ejemplo:
  // swCount = getSwitchCount();
  // hcsr04DistanceCm = getDistance();
}

void initializeJsons()
{
  // Inicializar el objeto JSON de envío
  sendJson["swPressed"]        = false;
  sendJson["swCount"]          = 0;
  sendJson["hcsr04DistanceCm"] = 0;

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

  // Enviar por serial
  serializeJson(sendJson, Serial);
  Serial.println();
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
    ledColor   = receiveJson["ledColor"].as<String>();

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
    Serial.println("Timeout de recepción");
  }
  else if (lastReceiveTime == 0)
  {
    // Si nunca se ha recibido nada, inicializar el tiempo
    lastReceiveTime = currentTime;
  }
}
