# HC-SR04 Library

Biblioteca para el control del sensor de distancia ultrasónico HC-SR04.

## Descripción

Esta librería proporciona una interfaz simple para controlar un sensor de distancia ultrasónico HC-SR04. Permite medir distancias de forma precisa y gestionar el estado del sensor entre modo de escaneo activo y modo de reposo.

## Estados del Sensor

```cpp
enum HCSR04_STATUS {
    HCS_IDLE,     // Sensor en reposo
    HCS_SCANNING  // Sensor activo midiendo
}
```

## Uso Básico

```cpp
#include <HCSR04.h>

// Crear instancia (TRIG en pin 7, ECHO en pin 8)
HCSR04 sensor(7, 8);

void setup() {
    Serial.begin(9600);
}

void loop() {
    // Iniciar medición
    sensor.startScanning();

    // Actualizar lecturas
    sensor.loop();

    // Obtener distancia
    uint8_t distancia = sensor.getDistance();
    Serial.println((String) "Distancia: " + distancia + " cm");

    // Detener medición
    sensor.stopScanning();
    delay(1000);
}
```

## Flujo de Operación

1. **Inicialización**:

   - Crear instancia especificando pines TRIG y ECHO
   - Los pines se configuran automáticamente

2. **Control de Operación**:

   - `startScanning()`: Inicia las mediciones
   - `stopScanning()`: Detiene las mediciones
   - `loop()`: Actualiza las lecturas

3. **Consulta de Datos**:
   - `getDistance()`: Obtiene la distancia en centímetros
   - `getStatus()`: Obtiene el estado actual del sensor
   - `statusToString()`: Convierte el estado a string para debug

## Parámetros Técnicos

### Timing del Sensor

- **Intervalo entre mediciones**: 10ms mínimo
- **Timeout de lectura**: 15ms (optimizado para aplicaciones de tiempo real)
- **Distancia máxima**: ~2.5 metros (con timeout de 15ms)
- **Distancia mínima**: ~2cm
- **Precisión**: ~3mm

### Configuración de Pines

- **TRIG**: Pin de salida para generar pulso ultrasónico
- **ECHO**: Pin de entrada para recibir eco
- **Voltaje**: 5V para alimentación

## Arquitectura del Código

La librería sigue un patrón de diseño simple y eficiente:

```cpp
void HCSR04::loop() {
    updateOutputs();  // Punto central de control
}

void HCSR04::updateOutputs() {
    if (this->status == HCS_SCANNING) {
        this->distance = getDistance();  // Lectura + cálculo + salida
    }
}
```

**Ventajas del diseño:**

- **Simplicidad**: Un solo punto de control
- **Eficiencia**: Sin overhead de múltiples métodos
- **Control**: Se ejecuta continuamente pero solo actúa cuando está escaneando

## Ejemplo de Implementación Completa

```cpp
#include <HCSR04.h>

HCSR04 sensor(7, 8);

void setup() {
    Serial.begin(9600);
}

void loop() {
    // Iniciar medición
    sensor.startScanning();

    // Actualizar lecturas
    sensor.loop();

    // Mostrar estado y distancia
    Serial.println((String) "Estado: " + statusToString(sensor.getStatus()));
    Serial.println((String) "Distancia: " + sensor.getDistance() + " cm");

    // Detener medición
    sensor.stopScanning();
    delay(1000);
}
```

## Integración con Otras Librerías

La librería HC-SR04 está diseñada para trabajar en conjunto con otras librerías del proyecto:

```cpp
#include <HCSR04.h>
#include <SensorServo.h>
#include <LEDRGB.h>

HCSR04 sensor(7, 8);
SensorServo servo(9);
LEDRGB led(4);

void setup() {
    servo.begin();
    led.begin();
}

void loop() {
    sensor.startScanning();
    sensor.loop();

    // Ejemplo de integración
    if (sensor.getDistance() < 20) {
        led.setRed();    // Objeto cercano
        servo.setAngle(90);  // Mirar al objeto
    } else {
        led.setBlue();   // Objeto lejano
        servo.setAngle(0);   // Mirar al frente
    }

    sensor.stopScanning();
    delay(100);
}
```

## Notas de Implementación

### Optimizaciones Realizadas

- **Timeout reducido**: 15ms en lugar de 30ms para mejor respuesta
- **Intervalo mínimo**: 10ms entre mediciones para estabilidad
- **Diseño simplificado**: Un solo punto de control en `updateOutputs()`

### Consideraciones de Uso

- El sensor debe estar en modo `SCANNING` para realizar mediciones
- Las mediciones se realizan automáticamente cada 10ms cuando está activo
- El timeout de 15ms limita la distancia máxima a ~2.5 metros
- Para distancias mayores, aumentar el timeout en el código fuente

### Debug y Monitoreo

- Los mensajes de debug están comentados por defecto
- Usar `getStatus()` para monitorear el estado del sensor
- `statusToString()` para conversión legible del estado
