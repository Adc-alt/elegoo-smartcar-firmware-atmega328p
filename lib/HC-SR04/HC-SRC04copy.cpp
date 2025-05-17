// #include <HC-SR04.h>
// // #include <ElegooSmartCar.h>

// HCSR04::HCSR04(uint8_t TRIG, uint8_t ECHO)
// {
//     this->pinTRIG = TRIG;
//     this->pinECHO = ECHO;

//     pinMode(TRIG, OUTPUT);
//     pinMode(ECHO, INPUT);
// };

// HCSR04_STATUS HCSR04::getStatus()
// {
//     return this->status;
// };

// void HCSR04::loop()
// {
//     // updateStatus();
//     updateOutputs();
// };

// void HCSR04::updateStatus()
// {
//     if (this->status == IDLE_HC)
//     {
//         return;
//     }
//     if (this->status == SCANNING_HC)
//     {
//         return;
//     }
// };

// void HCSR04::updateOutputs()
// {
//     if (this->status == SCANNING_HC)
//     {
//         unsigned long currentMillis = millis();

//         if (!started)
//         {
//             startTime = currentMillis;
//             started = true;
//             measured = false;
//         }

//         unsigned long elapsed = currentMillis - startTime;

//         if (elapsed < 2)
//         {
//             digitalWrite(this->pinTRIG, LOW);
//         }
//         else if (elapsed < 12)
//         {
//             Serial.println("12");
//             digitalWrite(this->pinTRIG, HIGH);
//         }
//         else if (!measured)
//         {
//             digitalWrite(TRIG_PIN, LOW);
//             long duration = pulseIn(ECHO_PIN, HIGH, 25000);
//             if (duration > 0)
//             {
//                 float distancia = duration * 0.034 / 2.0;

//                 // Guardar medida circular
//                 measures[index] = distancia;
//                 index = (index + 1) % NUM_MEASUREMENTS;

//                 // Calcular media
//                 float suma = 0;
//                 for (int i = 0; i < NUM_MEASUREMENTS; i++)
//                 {
//                     suma += measures[i];
//                 }
//                 distance = suma / NUM_MEASUREMENTS;

//                 Serial.print("Media: ");
//                 Serial.print(distance);
//                 Serial.println(" cm");
//             }
//             else
//             {
//                 Serial.println("Error: sin eco");
//             }

//             measured = true;
//         }
//         else if (elapsed > INTERVAL)
//         {
//             started = false; // reiniciar ciclo
//         }
//     }
// };

// uint8_t HCSR04::getDistance()
// {
//     // updateOutputs();
//     if (measured == true)
//     {
//         this->status = SCANNING_HC;
//         return this->distance;
//     }
// };

// void HCSR04::stopScanning()
// {
//     this->status = IDLE_HC;
// }