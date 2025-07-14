# LINE_TRACKING Library

Biblioteca para el control de seguimiento de línea usando sensores infrarrojos.

## Descripción

Esta librería proporciona una interfaz simple para controlar un sistema de seguimiento de línea usando tres sensores infrarrojos (izquierda, centro, derecha). El sistema detecta la posición de la línea y proporciona estados precisos para el control del vehículo.

## Características Principales

- **Detección precisa**: 8 estados diferentes para un control granular
- **Memoria de estado**: Mantiene la última dirección conocida cuando se pierde la línea
- **Configuración simple**: Solo requiere especificar los pines de los sensores
- **Lectura analógica**: Convierte automáticamente lecturas analógicas a digitales
- **Umbral configurable**: Umbral de detección ajustable mediante `DETECT_THRESHOLD`

## Estados del Sistema

```cpp
enum LINE_TRACKING_STATUS {
    CENTERED,              // Línea centrada (1 1 1) o (0 1 0)
    CENTERED_SLIGHT_LEFT,  // Línea ligeramente a la izquierda (0 1 1)
    CENTERED_SLIGHT_RIGHT, // Línea ligeramente a la derecha (1 1 0)
    LEFT,                  // Línea a la izquierda (1 0 0)
    RIGHT,                 // Línea a la derecha (0 0 1)
    LOST_PREV_LEFT,        // Perdido, último estado conocido a la izquierda
    LOST_PREV_RIGHT,       // Perdido, último estado conocido a la derecha
    LOST                   // No se detecta línea (0 0 0)
}
```

## Instalación

1. Copia la carpeta `LineTracking` a tu directorio `lib/`
2. Incluye la librería en tu sketch:

```cpp
#include <LineTracking.h>
```

## Uso Básico

```cpp
#include <LineTracking.h>

// Definir pines para los sensores (pines analógicos)
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
    lineTracker.loop();

    // Obtener estado actual
    LINE_TRACKING_STATUS status = lineTracker.getstatus();
    Serial.println("Estado: " + statusToString(status));

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
        case CENTERED_SLIGHT_LEFT:
            // Ajuste sutil a la derecha
            break;
        case CENTERED_SLIGHT_RIGHT:
            // Ajuste sutil a la izquierda
            break;
        case LOST_PREV_LEFT:
            // Buscar línea girando a la izquierda
            break;
        case LOST_PREV_RIGHT:
            // Buscar línea girando a la derecha
            break;
        case LOST:
            // Buscar línea (sin memoria de dirección)
            break;
    }
}
```

## API de la Librería

### Constructor

```cpp
LINE_TRACKING(uint8_t pinLeft, uint8_t pinMiddle, uint8_t pinRight)
```

- **pinLeft**: Pin analógico del sensor izquierdo
- **pinMiddle**: Pin analógico del sensor central
- **pinRight**: Pin analógico del sensor derecho

### Métodos Públicos

#### `void loop()`

Actualiza las lecturas de los sensores y el estado del sistema. **Debe ser llamado en cada iteración del loop principal.**

#### `LINE_TRACKING_STATUS getstatus()`

Retorna el estado actual del sistema de seguimiento de línea.

### Funciones Auxiliares

#### `String statusToString(LINE_TRACKING_STATUS status)`

Convierte el estado a string para debugging y logging.

## Lógica de Estados

### Estados Normales (Línea Detectada)

- **CENTERED**: Todos los sensores activos (1 1 1) o solo el central (0 1 0)
- **CENTERED_SLIGHT_LEFT**: Central y derecho activos (0 1 1)
- **CENTERED_SLIGHT_RIGHT**: Izquierdo y central activos (1 1 0)
- **LEFT**: Solo izquierdo activo (1 0 0)
- **RIGHT**: Solo derecho activo (0 0 1)

### Estados de Pérdida (Línea No Detectada)

- **LOST**: No se detecta línea (0 0 0) y no hay memoria de dirección previa
- **LOST_PREV_LEFT**: No se detecta línea, pero la última dirección conocida era izquierda
- **LOST_PREV_RIGHT**: No se detecta línea, pero la última dirección conocida era derecha

## Configuración

### Umbral de Detección

```cpp
#define DETECT_THRESHOLD 400 // Umbral para detección de línea
```

- **Valor por defecto**: 400
- **Rango recomendado**: 300-600
- **Ajuste**: Valores más bajos = más sensible, valores más altos = menos sensible

## Ejemplo de Implementación Completa

```cpp
#include <LineTracking.h>
#include <motor.h>

LINE_TRACKING lineTracker(A0, A1, A2);
MOTOR leftMotor(M_23_LEFT, LEFT_PWM, STBY);
MOTOR rightMotor(M_14_RIGHT, RIGHT_PWM, STBY);

void setup() {
    Serial.begin(9600);
}

void loop() {
    lineTracker.loop();
    LINE_TRACKING_STATUS status = lineTracker.getstatus();

    Serial.println("Estado: " + statusToString(status));

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
        case CENTERED_SLIGHT_LEFT:
            leftMotor.setSpeed(80);
            rightMotor.setSpeed(100);
            break;
        case CENTERED_SLIGHT_RIGHT:
            leftMotor.setSpeed(100);
            rightMotor.setSpeed(80);
            break;
        case LOST_PREV_LEFT:
            leftMotor.setSpeed(0);
            rightMotor.setSpeed(100);
            break;
        case LOST_PREV_RIGHT:
            leftMotor.setSpeed(100);
            rightMotor.setSpeed(0);
            break;
        case LOST:
            leftMotor.setSpeed(0);
            rightMotor.setSpeed(0);
            break;
    }
}
```

## Integración con Otras Librerías

La librería LINE_TRACKING está diseñada para trabajar en conjunto con otras librerías del proyecto:

```cpp
#include <LineTracking.h>
#include <SensorServo.h>
#include <LEDRGB.h>

LINE_TRACKING lineTracker(A0, A1, A2);
SENSORSERVO servo(9);
LED_RGB led(4);

void setup() {
    servo.begin();
    led.begin();
}

void loop() {
    lineTracker.loop();

    // Ejemplo de integración
    if (lineTracker.getstatus() == LOST ||
        lineTracker.getstatus() == LOST_PREV_LEFT ||
        lineTracker.getstatus() == LOST_PREV_RIGHT) {
        led.setRed();  // Indicar pérdida de línea
        servo.setAngle(90);  // Buscar línea
    } else {
        led.setBlue(); // Indicar línea encontrada
        servo.setAngle(0);   // Mirar al frente
    }
}
```

## Calibración y Ajuste

### Ajuste del Umbral

1. Coloca el robot sobre una línea negra
2. Lee los valores analógicos de cada sensor:

```cpp
Serial.print("Izquierdo: "); Serial.println(analogRead(A0));
Serial.print("Central: "); Serial.println(analogRead(A1));
Serial.print("Derecho: "); Serial.println(analogRead(A2));
```

3. Ajusta `DETECT_THRESHOLD` a un valor entre el máximo sobre línea blanca y el mínimo sobre línea negra

### Posicionamiento de Sensores

- Los sensores deben estar alineados horizontalmente
- La distancia entre sensores debe ser apropiada para el ancho de la línea
- Altura recomendada: 5-15mm sobre la superficie

## Solución de Problemas

### Problema: Detección inconsistente

- **Solución**: Ajustar `DETECT_THRESHOLD`
- **Solución**: Verificar la calibración de los sensores

### Problema: Cambios de estado muy frecuentes

- **Solución**: Implementar filtrado temporal en el código principal
- **Solución**: Aumentar `DETECT_THRESHOLD`

### Problema: No detecta líneas

- **Solución**: Verificar conexiones de los sensores
- **Solución**: Reducir `DETECT_THRESHOLD`
- **Solución**: Verificar la iluminación ambiental

## Notas Técnicas

- **Frecuencia de actualización**: Llamar `loop()` en cada iteración del loop principal
- **Lecturas analógicas**: Los sensores deben estar conectados a pines analógicos
- **Memoria de estado**: El sistema mantiene automáticamente el último estado conocido
- **Conversión digital**: Las lecturas analógicas se convierten a booleanos internamente
- **Estados persistentes**: Los estados LOST*PREV*\* se mantienen hasta detectar línea nuevamente

## Versión

- **Versión actual**: 1.0
- **Última actualización**: 2024
- **Compatibilidad**: Arduino IDE, PlatformIO
