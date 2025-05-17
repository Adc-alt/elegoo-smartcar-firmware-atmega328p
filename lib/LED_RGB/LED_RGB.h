#ifndef LED_RGB_H
#define LED_RGB_H

#include <Arduino.h>
#include <ElegooSmartCar.h>

enum LED_RGB_STATUS
{
    BLACK,
    BLUE,
    RED,
    YELOW,
    PURPLE
};

#define NUM_LEDS 1    // Este es el numero de LEDs que tiene el LED RGB
#define BRIGHTNESS 10 // Este es el porcentaje de brillo que tiene el LED
// Fijaros como en brightness el iluminado que hizo la librería ya puso
// un #define y mapeo entre 0 y 255 de 0 a 100 para que se puediera
//  usar de manera mas comoda tal y como hicimos en el motor

class LED_RGB
{
public:
    LED_RGB(uint8_t LED); // Declaración del constructor, declaro los parametros que necesito meter para cuando instancie la clase
    LED_RGB_STATUS getstatus();

    void begin();

    void setBlack();
    void setBlue();
    void setRed();
    void setYellow();
    void setPurple();

private:
    LED_RGB_STATUS status = BLACK;
    uint8_t pinRGB;

    void updateOutputs();
};

#endif