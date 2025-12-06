#ifndef LEDRGB_H
#define LEDRGB_H

#include <Arduino.h>
#include <elegoo_smart_car_lib.h>

// 1. Enums y constantes públicas
enum LED_RGB_STATUS
{
    BLACK,  // LED apagado
    BLUE,   // Color azul
    RED,    // Color rojo
    YELOW,  // Color amarillo
    PURPLE, // Color morado
    GREEN   // Color verde
};

// 2. Constantes de configuración
#define NUM_LEDS 1    // Número de LEDs en la tira
#define BRIGHTNESS 10 // Brillo del LED (0-255)
// Fijaros como en brightness el iluminado que hizo la librería ya puso
// un #define y mapeo entre 0 y 255 de 0 a 100 para que se puediera
//  usar de manera mas comoda tal y como hicimos en el motor

class LED_RGB
{
public:
    // 3. Constructor
    LED_RGB(uint8_t pinLED);

    // 4. Métodos públicos principales
    void inizializeLEDRGB();
    LED_RGB_STATUS getstatus();

    // 5. Setters de color
    void setBlack();
    void setBlue();
    void setRed();
    void setYellow();
    void setPurple();
    void setGreen();

private:
    // 6. Variables de estado
    LED_RGB_STATUS status = BLACK;
    uint8_t pinRGB;

    // 7. Métodos privados de actualización
    void updateOutputs();
};

// 8. Funciones auxiliares
String statusToString(LED_RGB_STATUS status);

#endif