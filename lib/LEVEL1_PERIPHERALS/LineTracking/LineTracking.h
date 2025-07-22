#ifndef LINE_TRACKING_H
#define LINE_TRACKING_H

#include <Arduino.h>
#include <ElegooSmartCar.h>

// 1. Enums y constantes públicas
enum LINE_TRACKING_STATUS
{
    CENTERED,              // Línea centrada
    CENTERED_SLIGHT_LEFT,  // Línea ligeramente a la izquierda
    CENTERED_SLIGHT_RIGHT, // Línea ligeramente a la derecha
    LEFT,                  // Línea a la izquierda
    RIGHT,                 // Línea a la derecha
    LOST_PREV_LEFT,        // Perdido, último estado conocido a la izquierda
    LOST_PREV_RIGHT,       // Perdido, último estado conocido a la derecha
    LOST                   // No se detecta línea
};

// 2. Constantes de configuración
#define DETECT_THRESHOLD 400 // Umbral para detección de línea

class LINE_TRACKING
{
public:
    // 3. Constructor
    LINE_TRACKING(uint8_t pinLeft, uint8_t pinMiddle, uint8_t pinRight);

    // 4. Métodos públicos principales
    LINE_TRACKING_STATUS getstatus();
    void loop();

private:
    // 5. Variables de estado
    LINE_TRACKING_STATUS status = LOST;
    LINE_TRACKING_STATUS prevStatus = LOST;

    // 6. Variables de configuración
    uint8_t pinLeft;
    uint8_t pinMiddle;
    uint8_t pinRight;

    // 7. Variables de lectura
    bool digitalLeftValue = false;
    bool digitalMiddleValue = false;
    bool digitalRightValue = false;

    // 8. Métodos privados de actualización
    void readInputs();
    void updateStatus();
};

// 9. Funciones auxiliares
String statusToString(LINE_TRACKING_STATUS status);

#endif