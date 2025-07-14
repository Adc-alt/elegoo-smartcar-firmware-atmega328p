#include "MyCar.h"

MyCar::MyCar(SENSORSERVO &sensorServo, MPUControl &mpuControl, BATTERY &battery, LINE_TRACKING &lineTracking, LED_RGB &ledRGB, MOTOR &leftMotor, MOTOR &rightMotor)
{
    this->sensorServo = &sensorServo;
    this->mpuControl = &mpuControl;
    this->battery = &battery;
    this->lineTracking = &lineTracking;
    this->ledRGB = &ledRGB;
    this->leftMotor = &leftMotor;
    this->rightMotor = &rightMotor;
}

void MyCar::loop()
{
    readInputs();
    updateStatus();
    updateOutputs();
}

void MyCar::readInputs()
{
    // LLamamos a los loops de las librerías pero si no se ha
    // comandado nada, nada funcionará, Ejemplo sensor servo, con el loop
    // estaríamos diciendo que chekee las salidas y los estados pero inicalmente todos esos
    // estados y salidas están inactivas IDLE si no hemos espeficifado nada
    this->sensorServo->loop();
    this->mpuControl->loop();
    this->battery->loop();
    this->lineTracking->loop();
}

void MyCar::updateStatus()
{
    // ===================== GESTION DE EMERGECIA =====================
    // MPUCONTROL_STATUS mpuStatus = this->mpuControl->getStatus();
    // Serial.println((String) "Este es el estado del MPU: " + (mpuStatus == LIFTED ? "LIFTED" : "NORMAL"));

    // if ((this->battery->getstatus() == EMERGENCY) || (this->mpuControl->getStatus() == LIFTED))
    // {
    //     Serial.println((String) "MyCar MODO_EMERGENCY " + statusToString(this->currentStatus));
    //     this->currentStatus = MODO_EMERGENCY;
    // }
    // if (this->battery->getstatus() == GOOD && this->mpuControl->getStatus() == NORMAL)
    // {
    //     Serial.println((String) "Salida de emergencia, volviendo a: " + statusToString(this->previousStatus));
    //     this->currentStatus = this->previousStatus;
    // }

    // ===================== GESTION DE EVASIÓN DE OBSTACULOS =====================

    // // OBSTACLE AVOIDANCE
    // if (isobstacleAvoidanceTrue && this->currentStatus != MODO_EMERGENCY && this->currentStatus != MODO_OBSTACLE_AVOIDANCE)
    // {
    //     this->currentStatus = MODO_OBSTACLE_AVOIDANCE;
    // }
}

void MyCar::updateOutputs()
{
    Serial.print("MyCar updateOutputs - currentStatus: " + statusToString(this->currentStatus));
    switch (this->currentStatus)
    {
        // case MODO_EMERGENCY:
        //     Serial.println("MyCar: ¡EJECUTANDO MODO_EMERGENCY!");
        //     this->leftMotor->forceStop();
        //     this->rightMotor->forceStop();
        //     this->ledRGB->setRed();
        //     Serial.println("LED configurado a ROJO");
        //     delay(1000);
        //     break;

        // case MODO_STOPPED:
        //     Serial.println("MyCar: ¡EJECUTANDO MODO_STOPPED!");
        //     this->leftMotor->freeStop();
        //     this->rightMotor->freeStop();
        //     this->ledRGB->setBlue();
        //     break;

    case MODO_OBSTACLE_AVOIDANCE:
        this->ledRGB->setPurple();
        // 1:

        break;
        // case MODO_MANUAL:
        //     // TODO: Implement manual mode
        //     break;
        // case MODO_LIFTED:
        //     // TODO: Implement lifted mode
        //     break;
        // case MODO_LINE_FOLLOWING:
        //     // TODO: Implement line following mode
        //     break;
        // case MODO_OBJECT_FOLLOWING:
        //     // TODO: Implement object following mode
        //     break;
        // case MODO_FACE_FOLLOWING:
        //     // TODO: Implement face following mode
        //     break;
        // case MODO_BALL_FOLLOWING:
        //     // TODO: Implement ball following mode
        //     break;
        // default:
        //     // Handle unknown status
        //     break;
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
    case MODO_MANUAL:
        return "MANUAL";
    case MODO_EMERGENCY:
        return "EMERGENCY";
    case MODO_LIFTED:
        return "LIFTED";
    case MODO_LINE_FOLLOWING:
        return "LINE_FOLLOWING";
    case MODO_OBJECT_FOLLOWING:
        return "OBJECT_FOLLOWING";
    case MODO_OBSTACLE_AVOIDANCE:
        return "OBSTACLE_AVOIDANCE";
    case MODO_FACE_FOLLOWING:
        return "FACE_FOLLOWING";
    case MODO_BALL_FOLLOWING:
        return "BALL_FOLLOWING";
    default:
        return "UNKNOWN";
    }
}