# LED_RGB Library

Biblioteca para el control de un LED RGB WS2812 usando la librería FastLED.

## Descripción

Esta librería proporciona una interfaz simple para controlar un LED RGB WS2812, permitiendo cambiar entre diferentes colores predefinidos y estados. Utiliza la librería FastLED para un control eficiente del LED.

## Estados del LED

```cpp
enum LED_RGB_STATUS {
    BLACK,  // LED apagado
    BLUE,   // Color azul
    RED,    // Color rojo
    YELLOW, // Color amarillo
    PURPLE  // Color morado
}
```

## Uso Básico

```cpp
#include <LED_RGB.h>

// Crear instancia (pin 4)
LED_RGB led(4);

void setup() {
    Serial.begin(9600);
    led.begin();
}

void loop() {
    // Cambiar colores
    led.setRed();
    delay(1000);
    led.setBlue();
    delay(1000);
    led.setYellow();
    delay(1000);
    led.setPurple();
    delay(1000);
    led.setBlack();
    delay(1000);
}
```

## Flujo de Operación

1. **Inicialización**:

   - Crear instancia especificando el pin del LED
   - Llamar a `begin()` en setup()

2. **Control de Colores**:

   - Usar los métodos setter para cambiar colores:
     - `setRed()`
     - `setBlue()`
     - `setYellow()`
     - `setPurple()`
     - `setBlack()`

3. **Consulta de Estado**:
   - `getStatus()`: Obtiene el estado actual del LED
   - `statusToString()`: Convierte el estado a string para debug

## Parámetros Ajustables

```cpp
#define NUM_LEDS 1     // Número de LEDs en la tira
#define BRIGHTNESS 10  // Brillo del LED (0-255)
```

## Notas Importantes

- Requiere la librería FastLED
- El pin debe ser compatible con la librería FastLED
- El brillo está predefinido a 10 para evitar consumo excesivo
- Los colores están predefinidos para consistencia

## Ejemplo de Implementación Completa

```cpp
#include <LED_RGB.h>

LED_RGB led(4);

void setup() {
    Serial.begin(9600);
    led.begin();
}

void loop() {
    // Ejemplo de uso con debug
    led.setRed();
    Serial.println((String) "Estado actual: " + statusToString(led.getStatus()));
    delay(1000);

    led.setBlue();
    Serial.println((String) "Estado actual: " + statusToString(led.getStatus()));
    delay(1000);

    // Apagar LED
    led.setBlack();
    Serial.println((String) "Estado actual: " + statusToString(led.getStatus()));
    delay(1000);
}
```

## Integración con Otras Librerías

La librería LED_RGB está diseñada para trabajar en conjunto con otras librerías del proyecto:

```cpp
#include <LED_RGB.h>
#include <SENSORSERVO.h>
#include <LINE_TRACKING.h>

LED_RGB led(4);
SENSORSERVO sensorServo(9);
LINE_TRACKING lineTracker(A0, A1, A2);

void setup() {
    led.begin();
    sensorServo.begin();
}

void loop() {
    // Ejemplo de integración
    if (lineTracker.getstatus() == LOST) {
        led.setRed();  // Indicar pérdida de línea
    } else {
        led.setBlue(); // Indicar línea encontrada
    }
}
```
