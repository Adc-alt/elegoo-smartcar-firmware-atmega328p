#include "MyCar.h"

MyCar::MyCar(SENSORSERVO &sensorServo, MPUControl &mpuControl, BATTERY &battery, LINE_TRACKING &lineTracking, LED_RGB &ledRGB, MOTOR &motor)
{
    this->sensorServo = &sensorServo;
    this->mpuControl = &mpuControl;
    this->battery = &battery;
    this->lineTracking = &lineTracking;
    this->ledRGB = &ledRGB;
    this->motor = &motor;
}

void MyCar::loop()
{
    this->sensorServo->loop();
    this->mpuControl->loop();
    this->battery->loop();
    this->lineTracking->loop();
    // this->ledRGB->loop();
    // this->motor->loop();
    updateStatus();
    updateOutputs();
}

void MyCar::updateStatus()
{
    if ((this->battery->getstatus() == EMERGENCY) || (this->mpuControl->getStatus() == LIFTED))
    {
        this->curretstatus = MODO_EMERGENCY;
    }
    else if (this->battery->getstatus() == GOOD)
    {
        this->curretstatus = previousStatus;
    }
}

void MyCar::updateOutputs()
{
    if (this->curretstatus == MODO_EMERGENCY)
    {
        this->motor->forceStop();
        this->ledRGB->setRed();
    }
    if (this->curretstatus == MODO_STOPPED)
    {
        this->motor->forceStop();
        this->ledRGB->setBlue();
    }
}

void MyCar::begin()
{
    // this->sensorServo->begin();
    // this->battery->begin();
    // this->lineTracking->begin();
    this->mpuControl->begin();
    this->ledRGB->begin();
}

// 1. Funciones auxiliares
String statusToString(MYCAR_STATUS status)
{
    switch (status)
    {
    case MODO_STOPPED:
        return "STOPPED";
    }
}