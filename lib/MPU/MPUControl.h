#ifndef MPUCONTROL_H
#define MPUCONTROL_H

#include <Arduino.h>
#include <Wire.h>    // Biblioteca para comunicación I2C: La librería ya sabe que pines usar
#include <MPU6050.h> // Librería para el sensor MPU6050

// 1. Estructuras y enums
struct MPUData
{
    float accelX, accelY, accelZ;
    float gyroX, gyroY, gyroZ;
};

// Eliminamos la definición duplicada
// MPU6050 mpu;

enum MPUCONTROL_STATUS
{
    LIFTED,
    NORMAL
};

// 2. Clase principal
class MPUControl
{
public:
    // 2.1 Constructor
    MPUControl(MPU6050 &mpu);

    // 2.2 Métodos públicos principales
    void loop();
    void begin();

    // 2.3 Getters
    MPUCONTROL_STATUS getStatus();
    MPUData getMPUData();

private:
    // 2.4 Variables de estado
    MPUCONTROL_STATUS status;
    MPUCONTROL_STATUS previousStatus;
    MPUCONTROL_STATUS nextStatus;

    // 2.5 Punteros a componentes
    MPU6050 *mpu;

    // 2.6 Variables de lectura
    float accelX, accelY, accelZ = 0;
    float gyroX, gyroY, gyroZ = 0;
    float angleX, angleY, angleZ = 0;
    unsigned long lastUpdate;

    // 2.7 Variables de calibración
    float accelOffsetX, accelOffsetY, accelOffsetZ = 0;
    float gyroOffsetX, gyroOffsetY, gyroOffsetZ = 0;

    // 2.8 Métodos privados
    void updateStatus();
    void readMPU();
    void calibrate();
    void calibrateGyro();
    unsigned long calculateMPUDelay(uint8_t currentAngle, uint8_t targetAngle);
};

// 3. Funciones auxiliares
String statusToString(MPUCONTROL_STATUS status);

#endif