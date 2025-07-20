#include "MPUControl.h"

// 1. Constructor
MPUControl::MPUControl(MPU6050 &mpu)
{
    this->mpu = &mpu;
    status = NORMAL;
}

// 2. Métodos públicos principales
void MPUControl::loop()
{
    readMPU();
    updateStatus();
}

void MPUControl::initializeMPU()
{
    Serial.println("[MPUControl] Calibrando sensor, por favor no mueva el coche...");
    calibrate();
    Serial.println("[MPUControl] Calibración completada.");
}

// 3. Getters
MPUCONTROL_STATUS MPUControl::getStatus()
{
    return this->status;
}

MPUData MPUControl::getMPUData()
{
    readMPU();
    MPUData data = {
        this->accelX,
        this->accelY,
        this->accelZ,
        this->gyroX,
        this->gyroY,
        this->gyroZ};
    return data;
}

// 4. Métodos privados de actualización
bool MPUControl::processMeasurement(float measurement)
{
    // Almacenar la medida en el buffer
    measurementBuffer[bufferIndex] = measurement;
    bufferIndex = (bufferIndex + 1) % BUFFER_SIZE;

    // Verificar si la medida indica que está levantado
    float deviation = abs(measurement - 1.0);
    bool isLifted = deviation > THRESHOLD;

    // Logging detallado para debug
    // Serial.print("MPU Debug - measurement: ");
    // Serial.print(measurement, 3);
    // Serial.print(" | deviation: ");
    // Serial.print(deviation, 3);
    // Serial.print(" | THRESHOLD: ");
    // Serial.print(THRESHOLD, 3);
    // Serial.print(" | isLifted: ");
    // Serial.print(isLifted ? "SI" : "NO");

    if (isLifted)
    {
        liftedCount++;
        normalCount = 0;
    }
    else
    {
        normalCount++;
        liftedCount = 0;
    }

    return isLifted;
}

void MPUControl::updateStatus()
{
    processMeasurement(this->accelZ);

    if (liftedCount >= CONFIRMATION_COUNT)
    {
        status = LIFTED;
    }
    else if (normalCount >= CONFIRMATION_COUNT)
    {
        status = NORMAL;
    }
}

void MPUControl::readMPU()
{
    int16_t ax, ay, az, gx, gy, gz;
    mpu->getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    // Normalizar y aplicar offset
    this->accelX = (ax / 16384.0) - this->accelOffsetX;
    this->accelY = (ay / 16384.0) - this->accelOffsetY;
    this->accelZ = (az / 16384.0) - this->accelOffsetZ;

    this->gyroX = (gx / 131.0) - this->gyroOffsetX;
    this->gyroY = (gy / 131.0) - this->gyroOffsetY;
    this->gyroZ = (gz / 131.0) - this->gyroOffsetZ;

    // Logging de valores
    // Serial.println("=== MPU6050 Readings ===");
    // Serial.println("Accelerometer (g):");
    // Serial.print("X: ");
    // Serial.print(this->accelX, 3);
    // Serial.print(" Y: ");
    // Serial.print(this->accelY, 3);
    // Serial.print(" Z: ");
    // Serial.println(this->accelZ, 3);

    // Serial.println("Gyroscope (deg/s):");
    // Serial.print("X: ");
    // Serial.print(this->gyroX, 3);
    // Serial.print(" Y: ");
    // Serial.print(this->gyroY, 3);
    // Serial.print(" Z: ");
    // Serial.println(this->gyroZ, 3);
    // Serial.println("=====================");
}

// 5. Métodos de calibración
void MPUControl::calibrate()
{
    const int samples = 100;
    long axSum = 0, aySum = 0, azSum = 0;
    long gxSum = 0, gySum = 0, gzSum = 0;

    for (int i = 0; i < samples; i++)
    {
        int16_t ax, ay, az, gx, gy, gz;
        mpu->getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
        axSum += ax;
        aySum += ay;
        azSum += az;
        gxSum += gx;
        gySum += gy;
        gzSum += gz;
        delay(10);
    }
    this->accelOffsetX = (axSum / (float)samples) / 16384.0;
    this->accelOffsetY = (aySum / (float)samples) / 16384.0;
    // Para Z, restar 1g (gravedad)
    this->accelOffsetZ = ((azSum / (float)samples) / 16384.0) - 1.0;

    // Calibración del giroscopio: restar el valor medio en reposo
    this->gyroOffsetX = (gxSum / (float)samples) / 131.0;
    this->gyroOffsetY = (gySum / (float)samples) / 131.0;
    this->gyroOffsetZ = (gzSum / (float)samples) / 131.0;
}

// 6. Funciones auxiliares
String statusToString(MPUCONTROL_STATUS status)
{
    switch (status)
    {
    case LIFTED:
        return "LIFTED";
    case NORMAL:
        return "NORMAL";
    default:
        return "UNKNOWN";
    }
}
