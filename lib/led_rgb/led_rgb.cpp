#include <FastLED.h>
#include <LED_RGB.h>

// 1. Variables globales
CRGB leds[NUM_LEDS];

// 2. Constructor
LED_RGB::LED_RGB(uint8_t pinLED)
{
  this->pinRGB = pinLED;
}

// 3. Métodos públicos principales
void LED_RGB::initializeLEDRGB()
{
  FastLED.addLeds<WS2812, RGB_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
}

LED_RGB_STATUS LED_RGB::getStatus()
{
  // Serial.println(statusToString(this->status));
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
  this->status = YELLOW;
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
void LED_RGB::setCyan()
{
  this->status = CYAN;
  this->updateOutputs();
}
void LED_RGB::setGray()
{
  this->status = GRAY;
  this->updateOutputs();
}
void LED_RGB::setSalmon()
{
  this->status = SALMON;
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
    case YELLOW:
      leds[0] = CRGB::Yellow;
      break;
    case PURPLE:
      leds[0] = CRGB::Purple;
      break;
    case GREEN:
      leds[0] = CRGB::Green;
      break;
    case CYAN:
      leds[0] = CRGB::Cyan;
      break;
    case GRAY:
      leds[0] = CRGB::Gray;
      break;
    case SALMON:
      leds[0] = CRGB(255, 100, 50);
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
    case YELLOW:
      return "YELLOW";
    case PURPLE:
      return "PURPLE";
    case GREEN:
      return "GREEN";
    case CYAN:
      return "CYAN";
    case GRAY:
      return "GRAY";
    case SALMON:
      return "SALMON";
    default:
      return "UNKNOWN";
  }
}
