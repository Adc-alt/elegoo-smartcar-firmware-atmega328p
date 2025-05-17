#include <LED_RGB.h>
#include <FastLED.h>

CRGB leds[NUM_LEDS];

// Definicion del constructor
LED_RGB::LED_RGB(uint8_t LEDD)
{
    this->pinRGB = LEDD; // Asingnación atemporal del pin
}

LED_RGB_STATUS LED_RGB::getstatus()
{
    return this->status;
    //En verdad se puede hacer un switch case para que devuelva el color en string
    //pregunta a curro si es legitimo
}

void LED_RGB::begin()
{
    FastLED.addLeds<WS2812, 4, GRB>(leds, 1); // Using pin 4 directly
    FastLED.setBrightness(BRIGHTNESS);
}

void LED_RGB::updateOutputs()
{
    if (this->status == BLACK)
    {
        leds[0] = CRGB::Black;
        FastLED.show();
    }
    if (this->status == BLUE)
    {
        leds[0] = CRGB::Blue; // Apagar el LED al inicio
        FastLED.show();
    }
    if (this->status == RED)
    {
        leds[0] = CRGB::Red; // Apagar el LED al inicio
        FastLED.show();
    }
    if (this->status == YELOW)
    {
        leds[0] = CRGB::Yellow; // Apagar el LED al inicio
        FastLED.show();
    }
    if (this->status == PURPLE)
    {
        leds[0] = CRGB::Purple; // Apagar el LED al inicio
        FastLED.show();
    }
}

void LED_RGB::setBlack()
{
    this->status = RED;
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


