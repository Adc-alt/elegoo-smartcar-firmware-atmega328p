#include "hcsr04.h"

// 1. Constructor
Hcsr04::Hcsr04(uint8_t trigPin, uint8_t echoPin)
{
  this->trigPin = trigPin;
  this->echoPin = echoPin;
}

void Hcsr04::begin()
{
  pinMode(this->trigPin, OUTPUT);
  pinMode(this->echoPin, INPUT);

  digitalWrite(this->trigPin, LOW);
}

// 2. Métodos públicos principales
/*
  update()
  --------
  Esta función se llama en cada vuelta del loop,
  pero NO mide siempre.

  Solo mide si ha pasado suficiente tiempo
  desde la última medición.
*/
void Hcsr04::update(TelemetryFrame& frame)
{
  // Tiempo actual
  uint32_t now = millis();

  // Si no han pasado 50ms, no hacemos nada
  // (evita medir demasiado rápido)
  if (now - lastScanTimeMs < 50)
  {
    return;
  }

  // Variables temporales
  bool valid = false;

  // Medimos la distancia
  uint16_t distanceCm = measureDistanceCm(valid);

  // Guardamos cuándo se midió
  lastScanTimeMs = now;

  // Guardamos el último valor internamente
  lastDistanceCm       = distanceCm;
  lastMeasurementValid = valid;

  // IMPORTANTE:
  // Aquí escribimos en el frame compartido
  // para que el TelemetrySender lo pueda enviar
  frame.hcsr04_distanceCm       = distanceCm;
  frame.hcsr04_measurementValid = valid;
}

/*
  measureDistanceCm()
  -------------------
  Hace la medición REAL del sensor.

  Esta función:
  - Manda el pulso TRIG
  - Espera la respuesta ECHO
  - Calcula la distancia
*/
uint16_t Hcsr04::measureDistanceCm(bool& valid)
{
  // 1) Enviamos un pulso corto por TRIG
  digitalWrite(this->trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(this->trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(this->trigPin, LOW);

  // 2) Esperamos el pulso de vuelta por ECHO
  // pulseIn devuelve el tiempo en microsegundos
  // Si pasa el timeout, devuelve 0
  unsigned long duration = pulseIn(echoPin, HIGH, 25000UL); // ~4 metros máx

  // Si no llegó nada → medición inválida
  if (duration == 0)
  {
    valid = false;
    return 0;
  }

  // Si llegó, es válida
  valid = true;

  // 3) Convertimos tiempo a centímetros
  // Regla aproximada: cm = microsegundos / 58
  return (uint16_t)(duration / 58UL);
}

// // 5. Funciones auxiliares
// String Hcsr04::distanceToString(uint8_t distance)
// {
//   return (String) "HCSR04: distance: " + distance;
// }
