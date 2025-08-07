# StateManager Module

## Descripción

El módulo StateManager proporciona un sistema completo de gestión de estados para el SmartCar, incluyendo control de botón físico y cambios de estado por software.

## Características

- **4 Estados del Sistema**: IDLE, ACTIVE, PROCESSING, EMERGENCY
- **Control por Botón Físico**: Con anti-rebote integrado
- **Control por Software**: Cambio de estados programático
- **Gestión de Transiciones**: Control seguro de cambios de estado
- **Configuración Flexible**: Pin de botón configurable

## Estados Disponibles

### STATE_IDLE (0)

- Estado de reposo del sistema
- Consumo mínimo de energía
- Esperando activación

### STATE_ACTIVE (1)

- Estado activo del sistema
- Funcionalidades principales operativas
- Control normal del vehículo

### STATE_PROCESSING (2)

- Estado de procesamiento de datos
- Análisis de sensores
- Toma de decisiones

### STATE_EMERGENCY (3)

- Estado de emergencia
- Parada de seguridad
- Funciones críticas activas

## Uso Básico

### Inicialización

```cpp
#include "StateManager/StateManager.h"

// Crear instancia (pin 2 por defecto)
STATE_MANAGER stateManager(2);

void setup() {
    stateManager.initialize();
}
```

### Loop Principal

```cpp
void loop() {
    stateManager.tick();

    // Obtener estado actual
    SYSTEM_STATE currentState = stateManager.getCurrentState();
}
```

## Métodos Públicos

### Control de Estados

- `setState(SYSTEM_STATE newState)` - Cambiar a estado específico
- `nextState()` - Pasar al siguiente estado
- `goToPreviousState()` - Pasar al estado anterior
- `getCurrentState()` - Obtener estado actual
- `stateToString(SYSTEM_STATE state)` - Convertir estado a string

### Control del Botón

- `isButtonPressed()` - Verificar si el botón está presionado
- `enableButtonControl(bool enable)` - Habilitar/deshabilitar control por botón
- `isButtonControlEnabled()` - Verificar si el control por botón está habilitado

## Ejemplos de Uso

### Cambio de Estado por Software

```cpp
// Cambiar directamente a un estado
stateManager.setState(STATE_ACTIVE);

// Pasar al siguiente estado
stateManager.nextState();

// Pasar al estado anterior
stateManager.goToPreviousState();
```

### Control Condicional

```cpp
void loop() {
    stateManager.tick();

    SYSTEM_STATE currentState = stateManager.getCurrentState();

    switch (currentState) {
        case STATE_IDLE:
            // Lógica para estado IDLE
            break;
        case STATE_ACTIVE:
            // Lógica para estado ACTIVE
            break;
        case STATE_PROCESSING:
            // Lógica para estado PROCESSING
            break;
        case STATE_EMERGENCY:
            // Lógica para estado EMERGENCY
            break;
    }
}
```

### Deshabilitar Control por Botón

```cpp
// Deshabilitar control por botón físico
stateManager.enableButtonControl(false);

// Solo control por software
stateManager.setState(STATE_ACTIVE);
```

## Configuración

### Pin del Botón

- **Por defecto**: Pin 2
- **Configurable**: Se puede cambiar en el constructor
- **Tipo**: INPUT_PULLUP (conectado a GND)

### Anti-rebote

- **Delay**: 50ms por defecto
- **Configurable**: Modificar `DEBOUNCE_DELAY` en el header

## Integración con Otros Módulos

El StateManager está diseñado para integrarse fácilmente con otros módulos del proyecto:

```cpp
// Ejemplo con motores
if (stateManager.getCurrentState() == STATE_ACTIVE) {
    leftMotor.forward(50);
    rightMotor.forward(50);
} else if (stateManager.getCurrentState() == STATE_EMERGENCY) {
    leftMotor.forceStop();
    rightMotor.forceStop();
}
```

## Estructura del Módulo

```
StateManager/
├── StateManager.h      # Header con declaraciones
├── StateManager.cpp    # Implementación
└── README.md          # Documentación
```

## Dependencias

- Arduino.h
- ElegooSmartCar.h

## Notas Técnicas

- **Thread-safe**: No (para Arduino)
- **Memory**: Bajo consumo
- **Performance**: Optimizado para Arduino
- **Debounce**: Hardware + Software
