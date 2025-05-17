#include "LINE_TRACKING.h"

// Definicion del constructor
LINE_TRACKING::LINE_TRACKING(uint8_t LEFT, uint8_t MIDDLE, uint8_t RIGHT)
{
    this->pinLEFT = LEFT;
    this->pinMIDDLE = MIDDLE;
    this->pinRIGHT = RIGHT;

    pinMode(pinLEFT, INPUT);
    pinMode(pinMIDDLE, INPUT);
    pinMode(pinRIGHT, INPUT);
}

LINE_TRACKING_STATUS LINE_TRACKING::getstatus()
{
    return this->status;
}

void LINE_TRACKING::updateOutputs()
{
    this->analogLeftValue = analogRead(this->pinLEFT);
    this->analogMiddleValue = analogRead(this->pinMIDDLE);
    this->analogRightValue = analogRead(this->pinRIGHT);
    Serial.println((String) "left: " + this->analogLeftValue + " middle: " + this->analogMiddleValue + " right: " + this->analogRightValue);
};

void LINE_TRACKING::tick()
{
    // Guardamos el estado actual antes de actualizarlo
    prevStatus = status;

    // Actualizamos las lecturas
    updateOutputs();
    convertAnalogToDigital();

    // Lógica de estados
    if (this->digitalLeftValue == 0 && this->digitalMiddleValue == 0 && this->digitalRightValue == 0)
    {
        // Si ya estamos en un estado LOST_PREV, lo mantenemos
        if (status == LOST_PREV_LEFT || status == LOST_PREV_RIGHT)
        {
            // Mantenemos el estado actual
            return;
        }

        // Si perdimos la línea por primera vez, usamos el estado previo para determinar la dirección
        if (prevStatus == RIGHT || prevStatus == CENTERED_SLIGHT_RIGHT)
        {
            this->status = LOST_PREV_RIGHT;
        }
        else if (prevStatus == LEFT || prevStatus == CENTERED_SLIGHT_LEFT)
        {
            this->status = LOST_PREV_LEFT;
        }
        else
        {
            this->status = LOST;
        }
    }
    else
    {
        // Si leemos algo diferente de 0 0 0, actualizamos el estado normalmente
        if (this->digitalLeftValue == 1 && this->digitalMiddleValue == 1 && this->digitalRightValue == 0)
        {
            this->status = CENTERED_SLIGHT_RIGHT;
        }
        if (this->digitalLeftValue == 0 && this->digitalMiddleValue == 1 && this->digitalRightValue == 1)
        {
            this->status = CENTERED_SLIGHT_LEFT;
        }

        if (this->digitalLeftValue == 0 && this->digitalMiddleValue == 0 && this->digitalRightValue == 1)
        {
            this->status = RIGHT;
        }

        if (this->digitalLeftValue == 1 && this->digitalMiddleValue == 0 && this->digitalRightValue == 0)
        {
            this->status = LEFT;
        }

        if (this->digitalLeftValue == 1 && this->digitalMiddleValue == 1 && this->digitalRightValue == 1||
        this->digitalLeftValue == 0 && this->digitalMiddleValue == 1 && this->digitalRightValue == 0)
        {
            this->status = CENTERED;
        }
    }
}

void LINE_TRACKING ::convertAnalogToDigital()
{
    if (this->analogLeftValue > DETECT_THRESHOLD)
    {
        this->digitalLeftValue = 1;
    }
    else
    {
        this->digitalLeftValue = 0;
    }
    if (this->analogMiddleValue > DETECT_THRESHOLD)
    {
        this->digitalMiddleValue = 1;
    }
    else
    {
        this->digitalMiddleValue = 0;
    }
    if (this->analogRightValue > DETECT_THRESHOLD)
    {
        this->digitalRightValue = 1;
    }
    else
    {
        this->digitalRightValue = 0;
    }
}
// void LINE_TRACKING::read()
// {
//     this->leftValue = analogRead(this->pinLEFT);
//     this->middleValue = analogRead(this->pinMIDDLE);
//     this->rightValue = analogRead(this->pinRIGHT);

// }