# CommandReceiver

## Descripción

`CommandReceiver` recibe comandos JSON del ESP32 por Serial y los convierte en un `CommandFrame` que puede ser ejecutado por los actuadores.

## Responsabilidades

- ✅ Leer JSON del Stream (Serial)
- ✅ Parsear el JSON manualmente (sin objetos anidados de ArduinoJson)
- ✅ Actualizar `CommandFrame` con los comandos recibidos

## No hace

- ❌ No ejecuta los comandos (eso lo hace `CommandExecutor`)
- ❌ No interpreta los comandos
- ❌ Solo recibe y actualiza el frame de comandos

## Flujo Completo del CommandReceiver

### PASO 1: Recibir Caracteres por Serial (líneas 95-104)

```cpp
while (in.available())  // ¿Hay datos en Serial?
{
  char c = in.read();    // Lee UN carácter
  
  if (!processingMessage)  // Si es el primer carácter del mensaje
  {
    processingMessage = true;  // Marca que empezamos a recibir
    bufferIndex = 0;           // Empieza desde el principio del buffer
    buffer[0] = '\0';          // Limpia el buffer
  }
```

**Ejemplo:** Llega `{"type":"command"...}` carácter por carácter:
- `{` → se guarda en `buffer[0]`
- `"` → se guarda en `buffer[1]`
- `t` → se guarda en `buffer[2]`
- etc.

### PASO 2: Detectar Fin de Mensaje (línea 106)

```cpp
if (c == '\n')  // ¿Llegó el salto de línea? (fin del mensaje)
```

Cuando llega `\n`, el mensaje completo está en `buffer`.

### PASO 3: Verificar que es un Comando (líneas 117-118)

```cpp
const char* typePos = findJsonKey(buffer, "type");  // Busca "type" en el JSON
if (typePos && strncmp(typePos, "\"command\"", 9) == 0)  // ¿Es "command"?
```

Busca `"type"` en el JSON y verifica que el valor sea `"command"`.

**Ejemplo:**
```json
{"type":"command","actuators":{...}}
```
Encuentra `"type"` y confirma que es `"command"` ✅

### PASO 4: Buscar "actuators" (líneas 125-126)

```cpp
const char* actuatorsPos = findJsonKey(buffer, "actuators");  // Busca "actuators"
if (actuatorsPos && *actuatorsPos == '{')  // ¿Encontró y es un objeto?
```

Busca `"actuators"` dentro del JSON.

### PASO 5: Parsear Cada Actuador

#### 5.1 Servo (líneas 128-138)

```cpp
const char* servoPos = strstr(actuatorsPos, "\"servo\"");  // Busca "servo"
if (servoPos)
{
  const char* anglePos = findJsonKey(servoPos, "angle");  // Busca "angle"
  if (anglePos)
  {
    commandFrame.servoHasCommand = true;
    commandFrame.servoAngle = extractIntValue(anglePos);  // Extrae el número
  }
}
```

**Ejemplo con:** `{"servo":{"angle":90}}`
- Busca `"servo"` → lo encuentra ✅
- Busca `"angle"` → lo encuentra ✅
- Extrae `90` → `commandFrame.servoAngle = 90` ✅

#### 5.2 Motores (líneas 140-177)

```cpp
const char* motorsPos = strstr(actuatorsPos, "\"motors\"");  // Busca "motors"
if (motorsPos)
{
  // Motor izquierdo
  const char* leftPos = strstr(motorsPos, "\"left\"");  // Busca "left"
  if (leftPos)
  {
    // Busca "action" y extrae el string
    const char* actionPos = findJsonKey(leftPos, "action");
    extractStringValue(actionPos, actionStr, ...);  // Extrae "forward"
    commandFrame.motors.leftAction = parseMotorAction(actionStr);  // Convierte a enum
    
    // Busca "speed" y extrae el número
    const char* speedPos = findJsonKey(leftPos, "speed");
    commandFrame.motors.leftSpeed = extractIntValue(speedPos);  // Extrae 100
  }
  
  // Lo mismo para "right"
}
```

**Ejemplo con:** `{"motors":{"left":{"action":"forward","speed":100}}}`
- Busca `"motors"` → lo encuentra ✅
- Busca `"left"` → lo encuentra ✅
- Busca `"action"` → extrae `"forward"` → convierte a `MotorAction::FORWARD` ✅
- Busca `"speed"` → extrae `100` → `commandFrame.motors.leftSpeed = 100` ✅

#### 5.3 LED (líneas 179-190)

```cpp
const char* ledPos = strstr(actuatorsPos, "\"led\"");  // Busca "led"
if (ledPos)
{
  const char* colorPos = findJsonKey(ledPos, "color");  // Busca "color"
  extractStringValue(colorPos, colorStr, ...);  // Extrae "red"
  commandFrame.ledColor = parseLedColor(colorStr);  // Convierte a enum
}
```

### PASO 6: Funciones Auxiliares

#### `findJsonKey()` (líneas 48-67)
Busca una clave en el JSON y retorna la posición del valor.

```cpp
findJsonKey(buffer, "servo")  // Busca "servo" en el JSON
// Retorna: posición donde está el valor (después del ":")
```

#### `extractIntValue()` (líneas 70-75)
Extrae un número desde una posición.

```cpp
extractIntValue("90}")  // Retorna: 90
```

#### `extractStringValue()` (líneas 78-91)
Extrae un string entre comillas.

```cpp
extractStringValue("\"forward\"", output, 16)  // Retorna: "forward" (sin comillas)
```

## Ejemplo Completo

### JSON Recibido:
```json
{"type":"command","actuators":{"servo":{"angle":90},"motors":{"left":{"action":"forward","speed":100}},"led":{"color":"red"}}}
```

### Flujo Paso a Paso:

1. **Recibe carácter por carácter** → guarda en `buffer`
2. **Detecta `\n`** → mensaje completo ✅
3. **Busca `"type"`** → encuentra `"command"` → es válido ✅
4. **Busca `"actuators"`** → encuentra el objeto ✅
5. **Busca `"servo"`** → encuentra `"angle":90` → `servoAngle = 90` ✅
6. **Busca `"motors"`** → encuentra `"left"` → `action="forward"`, `speed=100` ✅
7. **Busca `"led"`** → encuentra `"color":"red"` → `ledColor = RED` ✅
8. **Retorna `true`** → comando procesado ✅

## Resumen Visual

```
┌─────────────────────────────────────────────────────────────────┐
│                    FLUJO COMPLETO                                │
└─────────────────────────────────────────────────────────────────┘

Serial → Carácter por carácter → Buffer → Buscar claves → Extraer valores → CommandFrame
   ↓              ↓                  ↓           ↓                ↓              ↓
  "{"           buffer[0]        JSON      "servo"          "90"        servoAngle=90
  "t"           buffer[1]        completo  "angle"         (int)       ✅
  "y"           buffer[2]        en         "motors"        "forward"   leftAction=FORWARD
  ...           ...              buffer     "left"          (string)     ✅
  "\n"          FIN              ✅         "action"        "100"        leftSpeed=100
                                                                    ✅
```

## Formato de JSON Esperado

### Estructura General:
```json
{
  "type": "command",
  "actuators": {
    "servo": {
      "angle": 90
    },
    "motors": {
      "left": {
        "action": "forward",
        "speed": 100
      },
      "right": {
        "action": "reverse",
        "speed": 80
      }
    },
    "led": {
      "color": "red"
    }
  }
}
```

### Valores Válidos:

**Motor Actions:**
- `"forward"` → `MotorAction::FORWARD`
- `"reverse"` → `MotorAction::REVERSE`
- `"force_stop"` → `MotorAction::FORCE_STOP`
- `"free_stop"` → `MotorAction::FREE_STOP`

**LED Colors:**
- `"black"` → `LedColor::BLACK`
- `"blue"` → `LedColor::BLUE`
- `"red"` → `LedColor::RED`
- `"yellow"` → `LedColor::YELLOW`
- `"purple"` → `LedColor::PURPLE`
- `"green"` → `LedColor::GREEN`

**Servo Angle:**
- Número entero entre `0` y `180` (grados)

**Motor Speed:**
- Número entero entre `0` y `100` (porcentaje)

## Optimizaciones

### ¿Por qué Parseo Manual?

El parseo manual (sin objetos anidados de ArduinoJson) es necesario porque:

1. **Problema de RAM**: El ATmega328P tiene solo 2KB de RAM
2. **Overhead de objetos anidados**: ArduinoJson necesita mucho espacio para crear objetos anidados en memoria
3. **Solución**: Buscar las claves directamente en el string es mucho más eficiente en RAM

### Buffer Estático

- Usa `char buffer[256]` en lugar de `String`
- **Ventaja**: No fragmenta la memoria (no usa heap)
- **Ventaja**: RAM más contigua disponible para otros objetos

## Uso

```cpp
// En main.cpp
CommandReceiver commandReceiver(Serial);
CommandFrame commandFrame;

void loop() {
  // Recibir comandos
  if (commandReceiver.tryReceive(commandFrame)) {
    // Comando recibido y parseado
    // commandFrame ahora contiene los valores extraídos
  }
  
  // Ejecutar comandos (en CommandExecutor)
  commandExecutor.execute(commandFrame);
}
```

## Métodos Públicos

### `tryReceive(CommandFrame& commandFrame)`
- **Retorna**: `true` si recibió un comando válido, `false` en caso contrario
- **Efecto**: Actualiza `commandFrame` con los valores extraídos del JSON

### `checkTimeout(unsigned long timeoutMs)`
- **Propósito**: Limpia el buffer si no se recibe un mensaje completo en el tiempo especificado
- **Uso**: Llamar periódicamente para evitar buffers bloqueados

## Notas Técnicas

- El buffer tiene un tamaño máximo de 256 bytes
- Los mensajes deben terminar con `\n` (salto de línea)
- El parseo es case-sensitive (las claves deben coincidir exactamente)
- Si falta una clave, simplemente se ignora (no es error)

