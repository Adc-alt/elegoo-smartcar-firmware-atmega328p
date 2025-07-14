# Battery Library

Biblioteca para la monitorización y gestión del estado de la batería.

## Descripción

Esta librería proporciona una interfaz sencilla para medir el voltaje de la batería y determinar si el estado es bueno o de emergencia. Permite mostrar mensajes de advertencia y realizar acciones según el estado de la batería.

## Estados de la Batería

Podríamos poner más estados intermedios como warnings cuando la batería está a punto de caer por unos niveles pero no queremos sobre complicar las cosas.

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

BATTERY battery(A3); // Pin analógico para medir voltaje

void setup() {
    Serial.begin(9600);
}

void loop() {
    battery.loop(); // Actualiza estado y muestra mensajes
    float voltage = battery.getVoltage();
    Serial.println("Voltaje: " + String(voltage));
}
```

## Flujo de Operación

1. **Inicialización**:

   - Crear una instancia de `BATTERY` indicando el pin de lectura de voltaje.
   - El pin se configura automáticamente como entrada.

2. **Medición y Estado**:

   - `loop()`: Actualiza el estado de la batería cada `MEASURE_TIME` ms y muestra mensajes según el estado.
   - `getVoltage()`: Devuelve el voltaje actual de la batería (método privado).
   - `getstatus()`: Devuelve el estado actual (`GOOD` o `EMERGENCY`).
   - `printVoltage()`: Muestra el voltaje actual y estado por Serial.

3. **Mensajes**:
   - Si la batería está bien: `"[battery] Voltaje actual: X.XXV - Batería cargada completamente"`
   - Si la batería está baja: `"[battery] Voltaje actual: X.XXV - Carga la batería, hay un problema"`

## Parámetros Ajustables

```cpp
#define MEASURE_TIME 1000        // Tiempo entre mediciones en ms (1 segundo)
#define VOLTAGE_THRESHOLD 7.8    // Umbral de voltaje para estado GOOD/EMERGENCY
#define TOLERANCE 0.08           // Compensación por error estimado (8%)
```

## Notas Importantes

- El divisor de tensión debe estar correctamente dimensionado para no dañar el pin analógico.
- El umbral de emergencia está fijado en 7.8V, pero puede ajustarse según la batería utilizada.
- La función `getVoltage()` incluye una compensación por error estimado del 8%.
- La librería usa un divisor de tensión con R1=10kΩ y R2=1.5kΩ para medir voltajes de batería LiPo 2S.

## Ejemplo de Implementación Completa

```cpp
#include <Battery.h>

BATTERY battery(A3); // Pin A3 para medir voltaje

void setup() {
    Serial.begin(9600);
}

void loop() {
    battery.loop(); // Actualiza estado automáticamente

    if (battery.getstatus() == EMERGENCY) {
        // Apaga motores, enciende LED de aviso, etc.
        Serial.println("¡BATERÍA BAJA! Apagando sistemas...");
    }
}
```

## Funciones Disponibles

### Públicas

- `BATTERY(uint8_t pinVolt)`: Constructor
- `BATTERY_STATUS getstatus()`: Obtiene el estado actual
- `void loop()`: Función principal que debe llamarse en cada iteración
- `void printVoltage()`: Muestra voltaje y estado por Serial

### Privadas

- `float getVoltage()`: Lee y calcula el voltaje de la batería
