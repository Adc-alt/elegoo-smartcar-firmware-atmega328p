# MPU Control Library

Biblioteca para controlar el sensor MPU6050 en el Smart Car.

## Descripción / Description

Esta biblioteca proporciona una interfaz simple para:

- Detectar si el coche ha sido levantado
- Obtener lecturas del acelerómetro y giroscopio
- Calibración automática del sensor

This library provides a simple interface to:

- Detect if the car has been lifted
- Get accelerometer and gyroscope readings
- Automatic sensor calibration

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
    delay(100); //Lo necesitas para que el I2C tenga tiempo de inizializarse
    mpu.initialize();
    mpuControl.begin();  // Realiza la calibración automática
}
```

### Loop principal / Main loop

```cpp
void loop() {
    mpuControl.loop();

    // Obtener estado / Get status
    MPUCONTROL_STATUS status = mpuControl.getStatus();

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

## Calibración / Calibration

La calibración se realiza automáticamente al llamar a `begin()`. Durante la calibración:

- El coche debe estar inmóvil
- En una superficie plana
- No debe ser movido durante el proceso

## Autor / Author

Desarrollado para el Smart Car Project

## Licencia / License

Este proyecto está bajo la Licencia MIT
 