# MPU Control Library

Biblioteca para controlar el sensor MPU6050 en el Smart Car.

## Descripción / Description

Esta biblioteca proporciona una interfaz simple para:

- Detectar si el coche ha sido levantado
- Obtener lecturas del acelerómetro y giroscopio
- Calibración automática del sensor
- Filtrado de ruido con buffer de confirmación

This library provides a simple interface to:

- Detect if the car has been lifted
- Get accelerometer and gyroscope readings
- Automatic sensor calibration
- Noise filtering with confirmation buffer

## Instalación / Installation

1. Copiar la carpeta `MPU` en la carpeta `lib` de tu proyecto PlatformIO
2. Incluir la biblioteca en tu código:

```cpp
#include "MPUControl.h"
```

## Uso / Usage

### Inicialización / Initialization

```cpp
MPU6050 mpu;
MPUControl mpuControl(mpu);

void setup() {
    Wire.begin();
    delay(100); // Tiempo para que I2C se estabilice
    mpu.initialize();
    delay(100); // Tiempo para que el sensor se estabilice
    mpuControl.initializeMPU();  // Realiza la calibración automática
}
```

### Loop principal / Main loop

```cpp
void loop() {
    mpuControl.loop();

    // Obtener estado / Get status
    MPUCONTROL_STATUS status = mpuControl.getStatus();

    // Ejemplo de uso
    if (status == LIFTED) {
        Serial.println("Carro levantado!");
    } else {
        Serial.println("Carro en posición normal");
    }

    // Obtener datos del sensor / Get sensor data
    MPUData data = mpuControl.getMPUData();
}
```

## Estados / States

- `NORMAL`: El coche está en posición normal
- `LIFTED`: El coche ha sido levantado

## Estructura de datos / Data Structure

```cpp
struct MPUData {
    float accelX, accelY, accelZ;  // Acelerómetro en g
    float gyroX, gyroY, gyroZ;     // Giroscopio en grados/segundo
};
```

## API de la Librería

### Constructor

```cpp
MPUControl(MPU6050 &mpu)
```

- **mpu**: Referencia al objeto MPU6050

### Métodos Públicos

#### `void initializeMPU()`

Realiza la calibración automática del sensor. **Debe ser llamado después de `mpu.initialize()`**

#### `void loop()`

Actualiza las lecturas del sensor y el estado. **Debe ser llamado en cada iteración del loop principal**

#### `MPUCONTROL_STATUS getStatus()`

Retorna el estado actual del carro (NORMAL o LIFTED)

#### `MPUData getMPUData()`

Retorna los datos actuales del acelerómetro y giroscopio

### Funciones Auxiliares

#### `String statusToString(MPUCONTROL_STATUS status)`

Convierte el estado a string para debugging

## Configuración

### Parámetros Ajustables

```cpp
static const int BUFFER_SIZE = 5;        // Tamaño del buffer de confirmación
static constexpr float THRESHOLD = 0.10; // Umbral para detectar levantamiento
static const int CONFIRMATION_COUNT = 3; // Medidas consecutivas para confirmar
```

- **BUFFER_SIZE**: Número de medidas para el filtrado
- **THRESHOLD**: Sensibilidad de detección (0.10 = 10% de desviación)
- **CONFIRMATION_COUNT**: Medidas consecutivas necesarias para cambiar estado

## Calibración / Calibration

La calibración se realiza automáticamente al llamar a `initializeMPU()`. Durante la calibración:

- El coche debe estar inmóvil
- En una superficie plana
- No debe ser movido durante el proceso
- Se toman 100 muestras para calcular offsets

## Filtrado de Ruido

La librería implementa un sistema de filtrado para evitar falsos positivos:

1. **Buffer circular**: Almacena las últimas 5 medidas
2. **Confirmación**: Requiere 3 medidas consecutivas para cambiar estado
3. **Umbral configurable**: Detecta desviaciones del 10% por defecto

## Ejemplo de Implementación Completa

```cpp
#include "MPUControl.h"

MPU6050 mpu;
MPUControl mpuControl(mpu);

void setup() {
    Serial.begin(9600);

    // Inicialización en orden correcto
    Wire.begin();
    delay(100);
    mpu.initialize();
    delay(100);
    mpuControl.initializeMPU();

    Serial.println("MPU6050 inicializado y calibrado");
}

void loop() {
    mpuControl.loop();

    MPUCONTROL_STATUS status = mpuControl.getStatus();

    // Control basado en estado
    switch(status) {
        case LIFTED:
            Serial.println("¡Carro levantado! Deteniendo motores...");
            // Detener motores
            break;
        case NORMAL:
            Serial.println("Carro en posición normal");
            // Operación normal
            break;
    }

    // Obtener datos para debugging
    MPUData data = mpuControl.getMPUData();
    Serial.print("Acelerómetro Z: ");
    Serial.println(data.accelZ, 3);

    delay(100);
}
```

## Solución de Problemas

### Problema: Lecturas constantes

- **Solución**: Verificar que `mpuControl.loop()` se llame en cada iteración
- **Solución**: Verificar orden de inicialización

### Problema: Detección muy sensible

- **Solución**: Aumentar `THRESHOLD` en el código fuente
- **Solución**: Aumentar `CONFIRMATION_COUNT`

### Problema: No detecta levantamiento

- **Solución**: Verificar calibración
- **Solución**: Reducir `THRESHOLD`
- **Solución**: Verificar conexiones I2C

## Notas Técnicas

- **Frecuencia de actualización**: Llamar `loop()` en cada iteración del loop principal
- **Comunicación I2C**: Requiere Wire.begin() antes de la inicialización
- **Calibración**: Se realiza automáticamente en `initializeMPU()`
- **Filtrado**: Usa buffer circular y confirmación para estabilidad
- **Memoria**: ~200 bytes de RAM para buffers y variables

## Versión

- **Versión actual**: 1.0
- **Última actualización**: 2024
- **Compatibilidad**: Arduino IDE, PlatformIO

## Autor / Author

Desarrollado para el Smart Car Project

## Licencia / License

Este proyecto está bajo la Licencia MIT
