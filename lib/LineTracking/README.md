# LINE_TRACKING Library

Biblioteca para el control de seguimiento de línea usando sensores infrarrojos.

## Descripción

Esta librería proporciona una interfaz simple para controlar un sistema de seguimiento de línea usando tres sensores infrarrojos (izquierda, centro, derecha). El sistema detecta la posición de la línea y proporciona estados precisos para el control del vehículo.

## Estados del Sistema

```cpp
enum LINE_TRACKING_STATUS {
    CENTERED,              // Línea centrada
    CENTERED_SLIGHT_LEFT,  // Línea ligeramente a la izquierda
    CENTERED_SLIGHT_RIGHT, // Línea ligeramente a la derecha
    LEFT,                  // Línea a la izquierda
    RIGHT,                 // Línea a la derecha
    LOST_PREV_LEFT,        // Perdido, último estado conocido a la izquierda
    LOST_PREV_RIGHT,       // Perdido, último estado conocido a la derecha
    LOST                   // No se detecta línea
}
```

## Uso Básico

```cpp
#include <LINE_TRACKING.h>

// Definir pines para los sensores
const uint8_t PIN_LEFT = A0;
const uint8_t PIN_MIDDLE = A1;
const uint8_t PIN_RIGHT = A2;

// Crear instancia
LINE_TRACKING lineTracker(PIN_LEFT, PIN_MIDDLE, PIN_RIGHT);

void setup() {
    Serial.begin(9600);
}

void loop() {
    // Actualizar lecturas y estado
    lineTracker.tick();

    // Obtener estado actual
    LINE_TRACKING_STATUS status = lineTracker.getstatus();
    Serial.println((String) "Estado: " + statusToString(status));

    // Ejemplo de control básico
    switch(status) {
        case LEFT:
            // Girar a la izquierda
            break;
        case RIGHT:
            // Girar a la derecha
            break;
        case CENTERED:
            // Avanzar recto
            break;
        case LOST:
            // Buscar línea
            break;
    }
}
```

## Flujo de Operación

1. **Inicialización**:

   - Configurar pines de los sensores
   - Crear instancia de LINE_TRACKING

2. **Ciclo Principal**:

   - `tick()`: Actualiza lecturas y estado
   - `getstatus()`: Obtiene el estado actual
   - `statusToString()`: Convierte el estado a string para debug

3. **Estados de Detección**:
   - `CENTERED`: Línea perfectamente centrada
   - `CENTERED_SLIGHT_LEFT/RIGHT`: Línea ligeramente descentrada
   - `LEFT/RIGHT`: Línea claramente a un lado
   - `LOST`: No se detecta línea
   - `LOST_PREV_LEFT/RIGHT`: Mantiene memoria de la última dirección conocida

## Parámetros Ajustables

```cpp
#define DETECT_THRESHOLD 400  // Umbral para detección de línea
#define DEBUG_LINE_TRACKING 0 // Activar/desactivar mensajes de debug
```

## Notas Importantes

- Los sensores deben estar correctamente calibrados
- El umbral de detección puede necesitar ajuste según el tipo de línea y superficie
- El sistema mantiene memoria del último estado conocido para mejor recuperación
- Las lecturas analógicas se convierten a digitales internamente
- El sistema usa ternarios para una conversión eficiente de lecturas

## Ejemplo de Implementación Completa

```cpp
#include <LINE_TRACKING.h>
#include <motor.h>

LINE_TRACKING lineTracker(A0, A1, A2);
MOTOR leftMotor(M_23_LEFT, LEFT_PWM, STBY);
MOTOR rightMotor(M_14_RIGHT, RIGHT_PWM, STBY);

void setup() {
    Serial.begin(9600);
}

void loop() {
    lineTracker.tick();
    LINE_TRACKING_STATUS status = lineTracker.getstatus();

    Serial.println((String) "Estado: " + statusToString(status));

    switch(status) {
        case LEFT:
            leftMotor.setSpeed(0);
            rightMotor.setSpeed(100);
            break;
        case RIGHT:
            leftMotor.setSpeed(100);
            rightMotor.setSpeed(0);
            break;
        case CENTERED:
            leftMotor.setSpeed(100);
            rightMotor.setSpeed(100);
            break;
        case LOST_PREV_LEFT:
            leftMotor.setSpeed(0);
            rightMotor.setSpeed(100);
            break;
        case LOST_PREV_RIGHT:
            leftMotor.setSpeed(100);
            rightMotor.setSpeed(0);
            break;
        default:
            leftMotor.setSpeed(0);
            rightMotor.setSpeed(0);
    }
}
```

## Integración con Otras Librerías

La librería LINE_TRACKING está diseñada para trabajar en conjunto con otras librerías del proyecto:

```cpp
#include <LINE_TRACKING.h>
#include <SENSORSERVO.h>
#include <LED_RGB.h>

LINE_TRACKING lineTracker(A0, A1, A2);
SENSORSERVO servo(9);
LED_RGB led(4);

void setup() {
    servo.begin();
    led.begin();
}

void loop() {
    lineTracker.tick();

    // Ejemplo de integración
    if (lineTracker.getstatus() == LOST) {
        led.setRed();  // Indicar pérdida de línea
        servo.setAngle(90);  // Buscar línea
    } else {
        led.setBlue(); // Indicar línea encontrada
        servo.setAngle(0);   // Mirar al frente
    }
}
```
