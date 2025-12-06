#ifndef BATTERY_H
#define BATTERY_H

#include <Arduino.h>
#include <elegoo_smart_car_lib.h>

// 1. Enums y constantes públicas
enum BATTERY_STATUS
{
    GOOD,     // Batería en buen estado
    EMERGENCY // Batería baja, emergencia
};

// 2. Constantes de configuración
// #define MEASURE_TIME 120000 // 2 minutos
#define MEASURE_TIME 1000
#define VOLTAGE_THRESHOLD 7.8
#define TOLERANCE 0.08

class BATTERY
{
public:
    // 3. Constructor
    BATTERY(uint8_t pinVolt);

    // 4. Métodos públicos principales
    BATTERY_STATUS getstatus();
    void loop();

    // 5. Métodos privados de actualización
    float getVoltage();


private:
    // 5. Variables de estado
    BATTERY_STATUS status = GOOD;
    unsigned long lastMeasureTime = 0;
    float voltage = 0.0;

    // 6. Variables de configuración
    uint8_t pinVolt;    

};

#endif