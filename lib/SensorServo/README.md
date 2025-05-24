# SENSORSERVO Library

Biblioteca para el control de un servo motor con sensor ultrasónico HC-SR04.

## Descripción

Esta librería proporciona una interfaz para controlar un servo motor equipado con un sensor ultrasónico HC-SR04. Permite realizar dos modos principales de operación: búsqueda de objetos y escaneo del entorno. El sistema mantiene un control preciso del ángulo del servo y proporciona información sobre la posición de los objetos detectados.

## Estados del Sistema

```cpp
enum SENSORSERVO_STATUS {
    IDLE,     // REPOSO: Servo parado y HC-SR04 en reposo
    TURNING,  // GIRANDO: Servo girando hasta el próximo punto para escanear
    SCANNING, // BARRIENDO: Servo girando cada SCANNING_STEP grados y escaneando
    SEARCHING // BUSCANDO: Servo girando cada SEARCHING_STEP grados
}
```

## Uso Básico

```cpp
#include <sensorServo.h>
#include <Servo.h>
#include <HC-SR04.h>

// Crear instancias de los componentes
Servo servo;
HCSR04 sensor(TRIG_PIN, ECHO_PIN);

// Crear instancia del controlador
SENSORSERVO sensorServo(sensor, servo);

void setup() {
    Serial.begin(9600);
    servo.attach(SERVO_PIN);
}

void loop() {
    // Actualizar estado y salidas
    sensorServo.loop();

    // Iniciar búsqueda de objetos
    sensorServo.startSearching();

    // Obtener ángulo del objeto encontrado
    int objectAngle = sensorServo.getSearchAngle();
}
```

## Flujo de Operación

1. **Inicialización**:

   - Configurar servo y sensor HC-SR04
   - Crear instancia de SENSORSERVO

2. **Ciclo Principal**:

   - `loop()`: Actualiza estado y salidas
   - `startSearching()`: Inicia búsqueda de objetos
   - `startScanning()`: Inicia escaneo del entorno
   - `getSearchAngle()`: Obtiene ángulo del objeto encontrado

3. **Estados de Operación**:
   - `IDLE`: Sistema en reposo
   - `TURNING`: Servo en movimiento
   - `SCANNING`: Escaneo completo del entorno
   - `SEARCHING`: Búsqueda de objetos

## Parámetros Ajustables

```cpp
#define MIN_ANGLE 10
#define FRONT_ANGLE 90
#define MAX_ANGLE 150
#define SEARCHING_STEP 20
#define SEARCHING_THRESHOOLD 20
#define ANGLE_TIME 30
```

## Notas Importantes

- El servo debe estar correctamente conectado y calibrado
- El sensor HC-SR04 debe tener una línea de visión despejada
- El sistema calcula automáticamente el tiempo necesario para cada movimiento
- Las mediciones de distancia se realizan en centímetros

## Ejemplo de Implementación Completa

```cpp
#include <sensorServo.h>
#include <Servo.h>
#include <HC-SR04.h>

Servo servo;
HCSR04 sensor(TRIG_PIN, ECHO_PIN);
SENSORSERVO sensorServo(sensor, servo);

void setup() {
    Serial.begin(9600);
    servo.attach(SERVO_PIN);
}

void loop() {
    sensorServo.loop();

    // Ejemplo de búsqueda de objetos
    if (sensorServo.getStatus() == IDLE) {
        sensorServo.startSearching();
    }

    // Ejemplo de escaneo
    if (sensorServo.getStatus() == IDLE) {
        sensorServo.startScanning();
    }

    // Debug cada 2 segundos
    static unsigned long lastPrintTime = 0;
    if ((millis() - lastPrintTime) > 2000) {
        lastPrintTime = millis();
        Serial.println((String) "searchAngle: " + sensorServo.getSearchAngle());
    }
}
```

## Características Avanzadas

1. **Cálculo de Tiempo de Movimiento**:

   - El sistema calcula automáticamente el tiempo necesario para cada movimiento
   - Utiliza una fórmula sublineal para mayor precisión
   - Considera la distancia angular a recorrer

2. **Modo de Escaneo**:

   - Realiza mediciones en tres puntos: izquierda, centro y derecha
   - Proporciona un resumen de las distancias medidas
   - Útil para mapeo del entorno

3. **Modo de Búsqueda**:
   - Busca objetos en incrementos de SEARCHING_STEP grados
   - Se detiene cuando encuentra un objeto dentro del umbral
   - Mantiene memoria de la última posición conocida
