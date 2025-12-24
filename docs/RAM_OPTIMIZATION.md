# Optimización de RAM para ATmega328P

## Análisis del Uso de RAM

El ATmega328P tiene **solo 2KB (2048 bytes) de RAM total**, que se comparte entre:
- Variables globales
- Stack (llamadas a funciones)
- Heap (memoria dinámica)
- Buffers de librerías externas

## Consumidores Actuales de RAM

### 1. Buffers y Documentos JSON
- `CommandReceiver::buffer` (String): **128 bytes** (reservado)
- `CommandReceiver::StaticJsonDocument<128>`: **128 bytes**
- `TelemetrySender::StaticJsonDocument<300>`: **300 bytes**
- **Total JSON/Buffers: ~556 bytes**

### 2. Objetos Globales (estimado)
- `TelemetryFrame`: ~40 bytes
- `TelemetrySender`: ~10 bytes
- `CommandReceiver`: ~10 bytes
- `CommandFrame`: ~20 bytes
- `CommandExecutor`: ~20 bytes
- Sensores (SwitchButton, Hcsr04, IrSensor, Battery, LineSensor): ~100 bytes
- Actuadores (MOTOR x2, LED_RGB, Servo): ~50 bytes
- **Total objetos: ~270 bytes**

### 3. Arrays y Buffers Específicos
- `CRGB leds[1]`: **3 bytes** (FastLED)
- Variables internas de librerías (Servo, IRremote, MPU6050, FastLED): **~200-300 bytes**

### 4. Stack
- Llamadas a funciones anidadas: **~100-200 bytes**

### **TOTAL ESTIMADO: ~1200-1400 bytes de 2048 bytes**

## Estrategias de Optimización

### ✅ Ya Implementadas
1. ✅ Reducido `CommandReceiver::buffer` de 256 a 128 bytes
2. ✅ Reducido `CommandReceiver::StaticJsonDocument` de 256 a 128 bytes
3. ✅ Reducido `TelemetrySender::StaticJsonDocument` de 768 a 300 bytes

### 🔧 Optimizaciones Adicionales Recomendadas

#### 1. Reducir Tamaño del JSON de Telemetría
Si 300 bytes sigue siendo demasiado, opciones:
- **Opción A**: Reducir a 250 bytes y simplificar estructura
- **Opción B**: Construir JSON manualmente sin objetos anidados (más eficiente)
- **Opción C**: Enviar solo datos que han cambiado (differential updates)

#### 2. Optimizar CommandReceiver
- Si los comandos son siempre pequeños, reducir buffer a 64 bytes
- Reducir `StaticJsonDocument` a 96 bytes si es posible

#### 3. Usar PROGMEM para Strings Constantes
```cpp
// En lugar de:
const char* type = "telemetry_frame";

// Usar:
const char type[] PROGMEM = "telemetry_frame";
```

#### 4. Reducir Frecuencia de Envío
- Aumentar `INTERVAL` de 100ms a 200ms o más
- Reduce la presión sobre la RAM (menos objetos temporales)

#### 5. Deshabilitar Debug Serial
- Los `Serial.print()` crean strings temporales que consumen RAM
- Usar `F()` macro para strings constantes (ya lo haces en algunos lugares)

#### 6. Optimizar Librerías Externas
- **IRremote**: Ya optimizado con `DECODE_NEC` solo
- **FastLED**: Ya optimizado con `NUM_LEDS=1`
- **Servo**: Usar solo si es necesario
- **MPU6050**: Deshabilitado en el código (comentado)

#### 7. Usar Tipos de Datos Más Pequeños
- `uint8_t` en lugar de `int` cuando sea posible
- `bool` en lugar de `int` para flags

## Cálculo de RAM Disponible

```
RAM Total:           2048 bytes
─────────────────────────────────
JSON/Buffers:        -556 bytes
Objetos globales:    -270 bytes
Librerías:           -250 bytes
Stack:               -150 bytes
─────────────────────────────────
RAM Disponible:      ~822 bytes (margen de seguridad)
```

## Si Sigue Habiendo Overflow

### Solución 1: Construir JSON Manualmente
En lugar de usar `StaticJsonDocument` con objetos anidados, construir el JSON como string:

```cpp
void TelemetrySender::send(const TelemetryFrame& frame)
{
  // Construir JSON manualmente - mucho más eficiente en RAM
  out.print(F("{\"type\":\"telemetry\",\"seq\":"));
  out.print(frame.seq);
  out.print(F(",\"t_ms\":"));
  out.print(frame.t_ms);
  out.print(F(",\"sensors\":{"));
  // ... más campos
  out.println(F("}"));
}
```

### Solución 2: Formato Compacto (CSV o Binario)
En lugar de JSON, usar un formato más compacto:
- CSV: `type,seq,t_ms,sw_pressed,sw_count,...`
- Binario: Estructura fija sin overhead de JSON

### Solución 3: Enviar Solo Cambios
En lugar de enviar todos los datos cada vez, enviar solo los que han cambiado.

## Monitoreo de RAM

Para verificar el uso real de RAM, puedes agregar:

```cpp
void checkRAM() {
  extern int __heap_start, *__brkval;
  int freeRAM = (int) &freeRAM - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
  Serial.print(F("Free RAM: "));
  Serial.println(freeRAM);
}
```

## Prioridad de Optimizaciones

1. **ALTA**: Reducir tamaño de documentos JSON (ya hecho parcialmente)
2. **MEDIA**: Construir JSON manualmente si sigue fallando
3. **BAJA**: Cambiar formato de datos (solo si es absolutamente necesario)

