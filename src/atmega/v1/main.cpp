#include <Arduino.h>
#include <IRremote.hpp>
#include <elegoo_smart_car_lib.h>


#define IR_PIN 9

void setup()
{
  Serial.begin(9600);

  // Inicia el receptor IR
  IrReceiver.begin(IR_PIN, ENABLE_LED_FEEDBACK);

  Serial.println("IR receiver ready");
}

void loop()
{
  if (IrReceiver.decode())
  {
    // Lee el código recibido
    uint32_t code = IrReceiver.decodedIRData.decodedRawData;

    Serial.print("IR code: 0x");
    Serial.println(code, HEX);

    // Preparar para el siguiente
    IrReceiver.resume();
  }
}
