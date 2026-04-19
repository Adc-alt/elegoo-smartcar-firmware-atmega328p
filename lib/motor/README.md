# MOTOR Library

Biblioteca para el control de motores DC usando un driver L298N.

## Descripción

Esta librería proporciona una interfaz simple para controlar motores DC a través de un driver L298N. Permite controlar la dirección y velocidad del motor, así como implementar diferentes modos de parada. El sistema incluye protección contra sobrecargas y control preciso de la velocidad mediante PWM.

## Estados del Sistema

```cpp
enum MOTOR_STATUS {
    FORWARD,    // Motor girando hacia adelante
    REVERSE,    // Motor girando hacia atrás
    FORCE_STOP, // Parada forzada (frenado)
    FREE_STOP   // Parada libre (coasting)
}
```

## Uso Básico

```cpp
#include <motor.h>

// Definir pines para el motor
const uint8_t IN1 = 7;    // Pin de dirección 1
const uint8_t PWM = 5;    // Pin de velocidad (PWM)
const uint8_t STBY = 8;   // Pin de standby

// Crear instancia
MOTOR motor(IN1, PWM, STBY);

void setup() {
    // No se requiere configuración adicional
}

void loop() {
    // Ejemplo de control básico
    motor.forward(50);  // Avanzar al 50% de velocidad
    delay(1000);
    motor.forceStop();  // Parar el motor
    delay(1000);
    motor.reverse(30);  // Retroceder al 30% de velocidad
    delay(1000);
    motor.freeStop();   // Parada libre
}
```

## Flujo de Operación

1. **Inicialización**:

   - Configurar pines del motor
   - Crear instancia de MOTOR

2. **Ciclo Principal**:

   - `forward(vel)`: Control de avance
   - `reverse(vel)`: Control de retroceso
   - `forceStop()`: Parada forzada
   - `freeStop()`: Parada libre
   - `getStatus()`: Obtener estado actual

3. **Estados de Control**:
   - `FORWARD`: Motor girando hacia adelante
   - `REVERSE`: Motor girando hacia atrás
   - `FORCE_STOP`: Motor detenido con frenado
   - `FREE_STOP`: Motor en parada libre

## Parámetros Ajustables

```cpp
#define MIN_PWM 38   // Valor mínimo de PWM para movimiento
#define MAX_PWM 255  // Valor máximo de PWM (velocidad máxima)
```

## Notas Importantes

- El motor debe estar correctamente conectado al driver L298N
- La velocidad se especifica en porcentaje (0-100%)
- El sistema mapea automáticamente el porcentaje a valores PWM
- El pin STBY debe estar en HIGH para que el motor funcione
- La parada forzada (forceStop) es más abrupta que la parada libre (freeStop)

## Ejemplo de Implementación Completa

```cpp
#include <motor.h>

MOTOR leftMotor(M_23_LEFT, LEFT_PWM, STBY);
MOTOR rightMotor(M_14_RIGHT, RIGHT_PWM, STBY);

void setup() {
    // No se requiere configuración adicional
}

void loop() {
    // Ejemplo de control de dos motores
    leftMotor.forward(100);
    rightMotor.forward(100);
    delay(2000);

    leftMotor.forceStop();
    rightMotor.forceStop();
    delay(1000);

    leftMotor.reverse(50);
    rightMotor.reverse(50);
    delay(2000);

    leftMotor.freeStop();
    rightMotor.freeStop();
    delay(1000);
}
```

## Características Avanzadas

1. **Control de Velocidad**:

   - Mapeo automático de porcentaje a PWM
   - Valores PWM optimizados para el driver L298N
   - Control suave de aceleración y desaceleración

2. **Modos de Parada**:

   - `forceStop()`: Parada inmediata con frenado
   - `freeStop()`: Parada suave sin frenado
   - Protección contra sobrecargas

3. **Estado del Motor**:
   - Monitoreo constante del estado actual
   - Capacidad de obtener el estado en cualquier momento
   - Transiciones suaves entre estados
