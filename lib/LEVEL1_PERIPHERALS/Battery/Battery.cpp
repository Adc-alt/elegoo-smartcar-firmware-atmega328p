#include "Battery.h"

// 1. Constructor
BATTERY::BATTERY(uint8_t pinVolt)
{
    this->pinVolt = pinVolt;
    pinMode(pinVolt, INPUT);
}

// 2. Métodos públicos principales
BATTERY_STATUS BATTERY::getstatus()
{
    return this->status;
}

void BATTERY::loop()
{
    if (millis() - lastMeasureTime > MEASURE_TIME)
    {
        // printVoltage();
        lastMeasureTime = millis(); // En esta aplicacion no pasa nada pero millis se desborda después de 49 días. 4,294,967,295ms
        if (this->voltage > 7.8)
        {
            this->status = GOOD;
        }
        else
        {
            this->status = EMERGENCY;
        }
        this->voltage = getVoltage();
    }
}

float BATTERY::getVoltage()
{
    // Podríamos agregar un filtro simple para suavizar la lectura, con un filtro de 5 medidas sería suficiente
    // progongo también aunque en este caso igual no es necesario un filtro de Kalman, solo para que os suene
    // no voy a implementar nada de esto porque añade complejidad innecesaria pero me gusta tenerlo en mente que lo tengais en cuenta
    // ya que si por ejemplo estais trabajando en el sector aeroespacial, por poner un ejemplo, todo se mide al milimetros porque las aplicaciones
    // son criticas así que no esta mál que lo investigueis
    int analogValue = analogRead(pinVolt); // Expplicar que arduino usa un ADC de 10 bits, por lo que el valor máximo es 1023 y el voltaje de referencia es 5V
    // importante el voltaje maximo que se puede medir es el de referencia 5voltios por eso necesitamos el divisor de tensión y por eso se multiplica luego
    float voltage = analogValue * 5.0 / 1024.0 * (11.5 / 1.5); // (R1+R2)/R2 = 11.5/1.5
    voltage += voltage * TOLERANCE;                            // compensación por error estimado (8%)
    return voltage;
    Serial.println(String("battery Voltaje actual: ") + String(this->voltage) + ("V - ") + (this->status == GOOD ? "Batería cargada completamente" : "Carga la batería, hay un problema"));
}
