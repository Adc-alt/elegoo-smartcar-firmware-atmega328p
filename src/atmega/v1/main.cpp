#include <Arduino.h>
#include <FastLED.h>
#include <elegoo_smart_car_lib.h>

CRGB leds[NUM_LEDS];

// Colores FastLED a probar
const CRGB colors[] = {
  // CRGB::Pink,
  // CRGB::Yellow,
  // CRGB::Cyan,
  // // CRGB::Magenta,
  // // CRGB::Orange,
  // // CRGB::Purple,
  // CRGB::Aqua, CRGB::Lime, CRGB::Gold, CRGB::Teal, CRGB::Navy, CRGB::Maroon, CRGB::Olive, CRGB::Silver, CRGB::Gray};

  // Colores personalizados RGB (R, G, B)
  CRGB(255, 100, 50), CRGB(96, 0, 161)}; // Naranja intenso
// CRGB(100, 255, 200);  // Verde menta
// CRGB(200, 50, 255);   // Morado brillante
// CRGB(255, 200, 0);    // Amarillo dorado
// CRGB(50, 50, 255);    // Azul medio
// CRGB(255, 20, 147);   // Rosa profundo

// // Colores personalizados hexadecimales
// CRGB(0xFF6347); // Tomate (255, 99, 71)
// CRGB(0x9370DB); // Púrpura medio (147, 112, 219)
// CRGB(0x00CED1); // Turquesa oscuro (0, 206, 209)
// CRGB(0xFFD700); // Dorado (255, 215, 0)
// }
// ;

const int numColors = sizeof(colors) / sizeof(colors[0]);

void setup()
{
  Serial.begin(115200);
  Serial.println("FastLED color test ready");

  FastLED.addLeds<WS2812, RGB_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
}

void loop()
{
  for (int i = 0; i < numColors; i++)
  {
    leds[0] = colors[i];
    FastLED.show();
    Serial.print("Color ");
    Serial.print(i + 1);
    Serial.print("/");
    Serial.println(numColors);
    delay(3000);
  }
}
