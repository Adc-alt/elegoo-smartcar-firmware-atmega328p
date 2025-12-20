# TelemetryFrame

## Descripción

`TelemetryFrame` es la estructura de datos central que representa un snapshot coherente del estado de todos los sensores en un instante de tiempo.

## Concepto

Un **Frame** es una unidad de datos que:
- Representa un instante de tiempo
- Tiene timestamp y secuencia (clave)
- Es consumido por otro sistema
- No implica intención ni interpretación
- Es una unidad de datos en un stream (como un frame de video o red)

## Principios de Diseño

Este archivo define **EL MODELO DE DATOS** del sistema:

- ✅ **No hay lógica** - Solo estructura de datos
- ✅ **No hay hardware** - No depende de sensores específicos
- ✅ **No hay JSON** - La serialización está en `TelemetrySender`
- ✅ **Solo variables** - Representan el estado actual

## Flujo de Datos

### 1. El Frame es el Contenedor Único

```cpp
// En main.cpp
TelemetryFrame telemetryFrame;  // ← Este es el ÚNICO frame que existe
```

Este objeto vive durante todo el programa y almacena el estado actual de los sensores.

### 2. Los Sensores Escriben en el Frame

```cpp
// En loop()
switchButton.update(telemetryFrame);  // ← Escribe en el frame
mpu.update(telemetryFrame);           // ← Escribe en el frame
```

Los sensores reciben el frame **por referencia** (`TelemetryFrame&`), así que escriben directamente en el mismo objeto en memoria. **No se copia**.

### 3. El Sender Lee del Frame

```cpp
telemetrySender.trySend(telemetryFrame);  // ← Lee del frame y envía
```

El sender recibe el frame **por referencia** y lee los valores para construir el JSON.

### 4. ¿Se Pierden los Datos?

**NO**, porque:
- El frame es un objeto global que persiste
- Los sensores escriben en el mismo objeto
- El sender lee del mismo objeto
- Todo pasa por referencia, no hay copias

## Diagrama del Flujo

```
┌─────────────────────────────────────┐
│  TelemetryFrame telemetryFrame      │  ← Objeto único en memoria
│  (Estado actual de todos los sensores)│
└─────────────────────────────────────┘
           ↑                    ↓
           │                    │
    ┌──────┴──────┐      ┌──────┴──────┐
    │             │      │             │
┌───▼───┐    ┌───▼───┐  │  ┌─────────▼──┐
│Sensor1│    │Sensor2│  │  │   Sender   │
│update │    │update │  │  │  trySend   │
│(frame)│    │(frame)│  │  │  (frame)   │
└───────┘    └───────┘  │  └───────────┘
    │             │      │       │
    └─────────────┘      │       │
      (Escriben)         │       │
                         │       │
                         │       ▼
                         │   JSON → Serial
                         │
                    (Lee y envía)
```

## Estructura de Datos

### Metadatos
- `t_ms`: Timestamp en milisegundos
- `seq`: Contador de secuencia (incrementado solo cuando se envía)

### Sensores
- **Switch Button**: `sw_pressed`, `sw_count`
- **HCSR04**: `hcsr04_distanceCm`, `hcsr04_measurementValid`
- **IR Sensor**: `ir_new`, `ir_raw`, `ir_data`
- **Battery**: `battery_voltage`
- **Line Sensor**: `line_sensor_left`, `line_sensor_middle`, `line_sensor_right`
- **MPU**: `mpu_ax`, `mpu_ay`, `mpu_az`, `mpu_gx`, `mpu_gy`, `mpu_gz`

## Uso Típico

```cpp
// 1. Crear el frame (una sola vez, global)
TelemetryFrame telemetryFrame;

// 2. En el loop, actualizar sensores
void loop() {
  telemetryFrame.t_ms = millis();
  
  switchButton.update(telemetryFrame);  // Escribe datos
  mpu.update(telemetryFrame);           // Escribe datos
  // ... otros sensores
  
  // 3. Enviar (el sender lee del frame)
  telemetrySender.trySend(telemetryFrame);
}
```

## ¿Qué Pasa si No Pasas el Frame?

Si intentas hacer esto:

```cpp
// ❌ MAL - No pasas el frame
switchButton.update();  // Sin frame
mpu.update();          // Sin frame
telemetrySender.trySend();  // Sin frame
```

**Resultado:**
- Los sensores no sabrían dónde guardar los datos
- El sender no sabría qué enviar
- Los datos no se guardarían ni se enviarían

## Analogía

El frame es como una **pizarra compartida**:
- Los sensores escriben en la pizarra
- El sender lee de la pizarra y envía la información
- La pizarra se mantiene entre lecturas

## Separación de Responsabilidades

- **`telemetry_frame.h`**: Define el modelo de datos (estructura)
- **`telemetry_sender`**: Serializa a JSON y envía

Esta separación sigue el principio de **separación de datos y presentación**.

## Ventajas de Este Diseño

1. **Un solo punto de verdad**: Todos los datos están en un lugar
2. **Fácil de depurar**: Puedes inspeccionar el frame completo
3. **Eficiente en memoria**: No hay copias innecesarias
4. **Claro y simple**: El flujo es fácil de entender
5. **Extensible**: Agregar nuevos sensores es solo agregar campos al frame
