# JSON Processing with ArduinoJson

## Descripción

Este código demuestra cómo recibir y procesar JSON dinámicamente usando la librería ArduinoJson en un ATMEGA328P.

## Funcionalidad

### Recepción de JSON

- Recibe JSON por Serial (9600 baud)
- Espera hasta encontrar un salto de línea (`\n`)
- Elimina espacios y caracteres extra

### Procesamiento Dinámico

El código puede procesar **cualquier JSON** sin importar:

- Orden de los campos
- Campos faltantes o nuevos
- Tipos de datos
- Objetos anidados

## Flujo del Bucle For

### Estructura

```cpp
for (JsonPair kv : doc.as<JsonObject>()) {
    // Procesar cada campo
}
```

### Paso a Paso

#### 1. **Inicialización**

```cpp
for (JsonPair kv : doc.as<JsonObject>())
```

- Convierte `JsonDocument` a `JsonObject`
- Crea variable `kv` de tipo `JsonPair`
- Prepara iterador interno

#### 2. **Iteraciones**

Para JSON: `{"sensor":"gps","time":13511584,"battery":85}`

**Primera iteración:**

- `kv.key()` = `"sensor"`
- `kv.value()` = `"gps"`
- Imprime: `Campo: sensor = gps`

**Segunda iteración:**

- `kv.key()` = `"time"`
- `kv.value()` = `13511584`
- Imprime: `Campo: time = 13511584`

**Tercera iteración:**

- `kv.key()` = `"battery"`
- `kv.value()` = `85`
- Imprime: `Campo: battery = 85`

#### 3. **Finalización**

- No hay más elementos
- Sale del bucle
- Continúa con siguiente código

## Sistema de Detección de Tipos

### Tipos Soportados

#### 1. **Strings** (`const char*`)

```cpp
if (kv.value().is<const char*>()) {
    Serial.println(kv.value().as<const char*>());
}
```

**Ejemplo:** `"sensor":"gps"`

#### 2. **Enteros** (`int`)

```cpp
else if (kv.value().is<int>()) {
    Serial.println(kv.value().as<int>());
}
```

**Ejemplo:** `"battery":85`

#### 3. **Long** (`long`)

```cpp
else if (kv.value().is<long>()) {
    Serial.println(kv.value().as<long>());
}
```

**Ejemplo:** `"time":13511584`

#### 4. **Float** (`float`)

```cpp
else if (kv.value().is<float>()) {
    Serial.println(kv.value().as<float>());
}
```

**Ejemplo:** `"latitude":40.7128`

#### 5. **Objetos Anidados** (`JsonObject`)

```cpp
else if (kv.value().is<JsonObject>()) {
    Serial.println("[Objeto]");
    JsonObject nested = kv.value().as<JsonObject>();
    for (JsonPair nestedKv : nested) {
        Serial.print("  ");
        Serial.print(nestedKv.key().c_str());
        Serial.print(": ");
        Serial.println(nestedKv.value().as<float>());
    }
}
```

**Ejemplo:** `"coordinates":{"latitude":40.7128,"longitude":-74.0060}`

## Ventajas del Sistema

### 1. **Flexibilidad**

- No depende del orden de campos
- Maneja campos faltantes
- Acepta nuevos campos automáticamente

### 2. **Robustez**

- Detección automática de tipos
- Manejo de errores
- Procesamiento de objetos anidados

### 3. **Escalabilidad**

- Fácil agregar nuevos tipos
- Estructura modular
- Código reutilizable

## Ejemplos de Uso

### JSON Simple

```json
{ "sensor": "gps", "time": 13511584 }
```

### JSON Complejo

```json
{
  "sensor": "gps",
  "time": 13511584,
  "coordinates": {
    "latitude": 40.7128,
    "longitude": -74.006
  },
  "status": "active",
  "battery": 85
}
```

### JSON con Campos Nuevos

```json
{ "new_field": "value", "battery": 85, "custom_data": 123.45 }
```

## Cómo Probar

1. **Subir código** al ATMEGA328P
2. **Abrir Serial Monitor** (9600 baud)
3. **Enviar JSON** por Serial
4. **Ver resultado** en monitor

## Estructura del Proyecto

```
serial_json/
├── serial_json.cpp    # Código principal
└── README.md         # Esta documentación
```

## Dependencias

- `Arduino.h` - Funciones básicas de Arduino
- `ArduinoJson.h` - Procesamiento de JSON

## Notas Importantes

- **Memoria limitada** en ATMEGA328P
- **Tamaño máximo** de JSON depende de RAM disponible
- **Velocidad** de procesamiento es adecuada para aplicaciones básicas
- **Compatibilidad** con cualquier formato JSON válido
