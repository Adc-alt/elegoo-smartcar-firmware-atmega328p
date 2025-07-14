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
    NORMAL,
    LIFTED
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
    static const int BUFFER_SIZE = 5;        // Número de medidas para confirmar el estado
    static constexpr float THRESHOLD = 0.10; // Umbral para considerar un cambio (reducido de 0.15 a 0.10)
    static const int CONFIRMATION_COUNT = 3; // Número de medidas consecutivas necesarias para confirmar

    float measurementBuffer[BUFFER_SIZE];
    int bufferIndex = 0;
    int liftedCount = 0;
    int normalCount = 0;

    // 2.4 Variables de estado
    MPUCONTROL_STATUS status = NORMAL;
    MPUCONTROL_STATUS previousStatus;
    MPUCONTROL_STATUS nextStatus;

    // 2.5 Punteros a componentes
    MPU6050 *mpu;

    // 2.6 Variables de lectura
    float accelX, accelY, accelZ;
    float gyroX, gyroY, gyroZ;
    float angleX, angleY, angleZ = 0;
    unsigned long lastUpdate;

    // 2.7 Variables de calibración
    float accelOffsetX = 0, accelOffsetY = 0, accelOffsetZ = 0;
    float gyroOffsetX = 0, gyroOffsetY = 0, gyroOffsetZ = 0;

    // 2.8 Métodos privados
    void updateStatus();
    void readMPU();
    void calibrate();
    void calibrateGyro();
    unsigned long calculateMPUDelay(uint8_t currentAngle, uint8_t targetAngle);
    bool processMeasurement(float measurement);
};

// 3. Funciones auxiliares
String statusToString(MPUCONTROL_STATUS status);

#endif