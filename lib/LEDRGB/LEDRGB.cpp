#include "LEDRGB.h"
#include <FastLED.h>

// 1. Variables globales
CRGB leds[NUM_LEDS];

// 2. Constructor
LED_RGB::LED_RGB(uint8_t pinLED)
{
    this->pinRGB = pinLED;
}

// 3. Métodos públicos principales
void LED_RGB::begin()
{
    FastLED.addLeds<WS2812, 4, GRB>(leds, 1);
    FastLED.setBrightness(BRIGHTNESS);
}

LED_RGB_STATUS LED_RGB::getstatus()
{
    return this->status;
}

// 4. Setters de color
void LED_RGB::setBlack()
{
    this->status = BLACK;
    this->updateOutputs();
}

void LED_RGB::setBlue()
{
    this->status = BLUE;
    this->updateOutputs();
}

void LED_RGB::setRed()
{
    this->status = RED;
    this->updateOutputs();
}

void LED_RGB::setYellow()
{
    this->status = YELOW;
    this->updateOutputs();
}

void LED_RGB::setPurple()
{
    this->status = PURPLE;
    this->updateOutputs();
}
void LED_RGB::setGreen()
{
    this->status = GREEN;
    this->updateOutputs();
}

// 5. Métodos privados de actualización
void LED_RGB::updateOutputs()
{
    switch (this->status)
    {
    case BLACK:
        leds[0] = CRGB::Black;
        break;
    case BLUE:
        leds[0] = CRGB::Blue;
        break;
    case RED:
        leds[0] = CRGB::Red;
        break;
    case YELOW:
        leds[0] = CRGB::Yellow;
        break;
    case PURPLE:
        leds[0] = CRGB::Purple;
        break;
    case GREEN:
        leds[0] = CRGB::Green;
        break;
    }
    FastLED.show();
}

// 6. Funciones auxiliares
String statusToString(LED_RGB_STATUS status)
{
    switch (status)
    {
    case BLACK:
        return "BLACK";
    case BLUE:
        return "BLUE";
    case RED:
        return "RED";
    case YELOW:
        return "YELLOW";
    case PURPLE:
        return "PURPLE";
    case GREEN:
        return "GREEN";
    default:
        return "UNKNOWN";
    }
}
