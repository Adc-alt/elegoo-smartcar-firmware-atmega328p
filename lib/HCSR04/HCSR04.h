#ifndef HCSR04_H
#define HCSR04_H

#include <Arduino.h>

// 1. Enums y constantes públicas
enum HCSR04_STATUS
{
    HCS_IDLE,    // Sensor en reposo
    HCS_SCANNING // Sensor activo midiendo
};

// 2. Constantes de configuración
#define DEBUG_HCSR04 0 // Activar/desactivar mensajes de debug

class HCSR04
{
public:
    // 3. Constructor
    HCSR04(uint8_t TRIG, uint8_t ECHO);

    // 4. Métodos públicos principales
    HCSR04_STATUS getStatus();
    uint8_t getDistance();
    void loop();

    // 5. Control de operación
    void startScanning();
    void stopScanning();

private:
    // 6. Variables de estado
    HCSR04_STATUS status = HCS_IDLE;
    uint8_t pinTRIG;
    uint8_t pinECHO;
    uint8_t distance = 0;
    unsigned long startScanningTime = 0;

    // 7. Métodos privados de actualización
    void updateOutputs();
};

// 8. Funciones auxiliares
String statusToString(HCSR04_STATUS status);

#endif
