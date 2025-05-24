# HC-SR04 Library

Biblioteca para el control del sensor de distancia ultrasónico HC-SR04.

## Descripción

Esta librería proporciona una interfaz simple para controlar un sensor de distancia ultrasónico HC-SR04. Permite medir distancias de forma precisa y gestionar el estado del sensor entre modo de escaneo activo y modo de reposo.

## Estados del Sensor

```cpp
enum HCSR04_STATUS {
    HCS_IDLE,     // Sensor en reposo
    HCS_SCANNING  // Sensor activo midiendo
}
```

## Uso Básico

```cpp
#include <HC-SR04.h>

// Crear instancia (TRIG en pin 7, ECHO en pin 8)
HCSR04 sensor(7, 8);

void setup() {
    Serial.begin(9600);
}

void loop() {
    // Iniciar medición
    sensor.startScanning();

    // Actualizar lecturas
    sensor.loop();

    // Obtener distancia
    uint8_t distancia = sensor.getDistance();
    Serial.println((String) "Distancia: " + distancia + " cm");

    // Detener medición
    sensor.stopScanning();
    delay(1000);
}
```

## Flujo de Operación

1. **Inicialización**:

   - Crear instancia especificando pines TRIG y ECHO
   - Los pines se configuran automáticamente

2. **Control de Operación**:

   - `startScanning()`: Inicia las mediciones
   - `stopScanning()`: Detiene las mediciones
   - `loop()`: Actualiza las lecturas

3. **Consulta de Datos**:
   - `getDistance()`: Obtiene la distancia en centímetros
   - `getStatus()`: Obtiene el estado actual del sensor
   - `statusToString()`: Convierte el estado a string para debug

## Parámetros Ajustables

```cpp
#define DEBUG_HCSR04 0  // Activar/desactivar mensajes de debug
```

## Notas Importantes

- El sensor requiere alimentación de 5V
- La distancia máxima de medición es de aproximadamente 400cm
- La distancia mínima de medición es de aproximadamente 2cm
- El tiempo entre mediciones debe ser al menos 10ms
- La precisión es de aproximadamente 3mm

## Ejemplo de Implementación Completa

```cpp
#include <HC-SR04.h>

HCSR04 sensor(7, 8);

void setup() {
    Serial.begin(9600);
}

void loop() {
    // Iniciar medición
    sensor.startScanning();

    // Actualizar lecturas
    sensor.loop();

    // Mostrar estado y distancia
    Serial.println((String) "Estado: " + statusToString(sensor.getStatus()));
    Serial.println((String) "Distancia: " + sensor.getDistance() + " cm");

    // Detener medición
    sensor.stopScanning();
    delay(1000);
}
```

## Integración con Otras Librerías

La librería HC-SR04 está diseñada para trabajar en conjunto con otras librerías del proyecto:

```cpp
#include <HC-SR04.h>
#include <SENSORSERVO.h>
#include <LED_RGB.h>

HCSR04 sensor(7, 8);
SENSORSERVO servo(9);
LED_RGB led(4);

void setup() {
    servo.begin();
    led.begin();
}

void loop() {
    sensor.startScanning();
    sensor.loop();

    // Ejemplo de integración
    if (sensor.getDistance() < 20) {
        led.setRed();    // Objeto cercano
        servo.setAngle(90);  // Mirar al objeto
    } else {
        led.setBlue();   // Objeto lejano
        servo.setAngle(0);   // Mirar al frente
    }

    sensor.stopScanning();
    delay(100);
}
```
