# Battery Library

Biblioteca para la monitorización y gestión del estado de la batería.

## Descripción

Esta librería proporciona una interfaz sencilla para medir el voltaje de la batería y determinar si el estado es bueno o de emergencia. Permite mostrar mensajes de advertencia y realizar acciones según el estado de la batería.

## Estados de la Batería

```cpp
enum BATTERY_STATUS
{
    GOOD,      // Batería en buen estado
    EMERGENCY  // Batería baja, emergencia
};
```

## Uso Básico

```cpp
#include <Battery.h>

BATTERY battery(VOLTAGE_PIN);

void setup() {
    Serial.begin(9600);
}

void loop() {
    battery.tick();
    float voltage = battery.getVoltage();
    Serial.println((String) "Voltaje: " + voltage);
}
```

## Flujo de Operación

1. **Inicialización**:

   - Crear una instancia de `BATTERY` indicando el pin de lectura de voltaje.
   - El pin se configura automáticamente como entrada.

2. **Medición y Estado**:

   - `tick()`: Actualiza el estado de la batería y muestra mensajes según el estado.
   - `getVoltage()`: Devuelve el voltaje actual de la batería.
   - `getstatus()`: Devuelve el estado actual (`GOOD` o `EMERGENCY`).

3. **Mensajes**:
   - Si la batería está bien: `"Batería cargada completamente"`
   - Si la batería está baja: `"CARGA LA BATERÍA HAY UN PROBLEMA"`

## Parámetros Ajustables

```cpp
#define VOLTAGE_PIN A3  // Pin analógico para medir el voltaje
#define MEASURE_TIME 1000 // Tiempo entre mediciones en ms
```

## Notas Importantes

- El divisor de tensión debe estar correctamente dimensionado para no dañar el pin analógico.
- El umbral de emergencia está fijado en 7.8V, pero puede ajustarse según la batería utilizada.
- La función `getVoltage()` incluye una compensación por error estimado del 8%.

## Ejemplo de Implementación Completa

```cpp
#include <Battery.h>

BATTERY battery(VOLTAGE_PIN);

void setup() {
    Serial.begin(9600);
}

void loop() {
    battery.tick();
    float voltage = battery.getVoltage();
    Serial.println((String) "Voltaje: " + voltage);

    if (battery.getstatus() == EMERGENCY) {
        // Apaga motores, enciende LED de aviso, etc.
    }
}
```
