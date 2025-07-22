# MyCar - Sistema de Control Principal del Coche Inteligente

## Concepto General

MyCar es el **cerebro central** de un coche inteligente basado en Arduino (ATmega328P). Actúa como un **orquestador** que coordina todos los módulos y sensores del vehículo, gestionando los diferentes modos de operación y las transiciones entre ellos.

## Arquitectura Conceptual

### 🎯 **Propósito Principal**

- **Gestión de Estados**: Controla qué modo está activo en cada momento
- **Coordinación de Módulos**: Orquesta la comunicación entre todos los componentes
- **Lógica de Negocio**: Implementa la inteligencia del vehículo

### 🔧 **Componentes Integrados**

MyCar coordina estos módulos:

- **Sensores**: Servo para escaneo, MPU para orientación, batería para energía
- **Actuadores**: Motores izquierdo/derecho, LED RGB para feedback visual
- **Sistemas**: Seguimiento de línea, control por infrarrojos
- **Navegación**: Seguimiento de objetos, caras, pelotas, evasión de obstáculos

## Modos de Operación

### 🚗 **Modos Principales**

1. **STOPPED** - Vehículo detenido, modo seguro
2. **MANUAL** - Control manual por infrarrojos
3. **EMERGENCY** - Parada de emergencia (batería baja, vehículo levantado)
4. **LIFTED** - Detección de que el vehículo está en el aire
5. **LINE_FOLLOWING** - Seguimiento automático de líneas
6. **OBJECT_FOLLOWING** - Seguimiento de objetos
7. **OBSTACLE_AVOIDANCE** - Evasión automática de obstáculos
8. **FACE_FOLLOWING** - Seguimiento de rostros
9. **BALL_FOLLOWING** - Seguimiento de pelotas

### 🔄 **Ciclo de Vida**

```
INPUTS → PROCESSING → OUTPUTS
   ↓         ↓          ↓
Lectura → Análisis → Acción
```

## Comportamiento Esperado

### 🧠 **Inteligencia del Sistema**

- **Prioridad de Emergencia**: Siempre responde a condiciones críticas
- **Transiciones Suaves**: Cambia entre modos de forma controlada
- **Feedback Visual**: LED RGB indica el estado actual
- **Autonomía**: Funciona de forma independiente una vez configurado

### 🛡️ **Seguridad**

- **Detección de Levantamiento**: Se detiene si detecta que está en el aire
- **Control de Batería**: Entra en emergencia si la batería está baja
- **Parada de Emergencia**: Respuesta inmediata a condiciones peligrosas

### 🎮 **Control**

- **Manual**: Control remoto por infrarrojos
- **Automático**: Diferentes modos de navegación autónoma
- **Híbrido**: Combinación de control manual y automático

## Filosofía de Diseño

### 🎯 **Principios**

1. **Modularidad**: Cada componente tiene responsabilidades específicas
2. **Escalabilidad**: Fácil agregar nuevos modos o sensores
3. **Robustez**: Manejo de errores y estados de emergencia
4. **Simplicidad**: Interfaz clara y fácil de entender

### 🔄 **Flujo de Datos**

```
Sensores → MyCar → Actuadores
   ↓        ↓         ↓
Lectura → Decisión → Acción
```

## Casos de Uso

### 🏠 **Doméstico**

- Seguimiento de líneas para limpieza
- Evasión de obstáculos en casa
- Control remoto para entretenimiento

### 🎓 **Educativo**

- Aprendizaje de robótica
- Programación de comportamientos
- Experimentación con sensores

### 🔬 **Investigación**

- Prototipado de algoritmos de navegación
- Pruebas de sensores
- Desarrollo de comportamientos autónomos

---

_MyCar representa la evolución de un simple coche de juguete hacia un vehículo inteligente y autónomo, demostrando los principios fundamentales de la robótica moderna._
