# 🚗 SmartCar ATMEGA328P - Control IR Inteligente

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![ATMEGA328P](https://img.shields.io/badge/ATMEGA328P-AVR-orange)](https://www.microchip.com/wwwproducts/en/ATmega328P)
[![PlatformIO](https://img.shields.io/badge/PlatformIO-IDE-blue)](https://platformio.org/)

<div align="center">
  <img src="https://img.shields.io/badge/SmartCar-IR%20Control-brightgreen" alt="SmartCar IR Control"/>
</div>

## 🎯 Descripción

Este proyecto implementa un sistema de control inteligente para un SmartCar basado en ATMEGA328P, con un **sistema de control IR avanzado** que resuelve uno de los mayores desafíos en robótica móvil: **el control suave y responsivo**.

## 🔧 El Rompecabezas del Control IR

### 🧩 **La Pieza Clave: El Problema del "Pulso Único"**

El control IR tradicional tiene una limitación fundamental que lo hace difícil de usar:

```
❌ PROBLEMA TRADICIONAL:
Presionas FORWARD → Envía 1 señal → Carro avanza
Mantienes presionado → NO envía más señales → Carro se detiene
Resultado: Control muy difícil y frustrante
```

### ✅ **Nuestra Solución: Sistema de Timeout Inteligente**

Hemos implementado un **sistema de timeout inteligente** que transforma el control IR en una experiencia fluida:

```cpp
// Variables clave del sistema
unsigned long lastIRTime = 0;           // Última señal recibida
const unsigned long IR_TIMEOUT = 150;   // Timeout de 150ms
```

### 🔄 **Cómo Funciona el Sistema**

#### **1. Recepción de Señales Continuas**

```cpp
if (IrReceiver.decode())
{
    lastIRTime = millis();  // Actualiza tiempo de última señal
    // Procesa comando (FORWARD, BACKWARD, etc.)
}
```

#### **2. Mantenimiento del Estado**

```cpp
else
{
    // Auto-reset después de timeout
    if (millis() - lastIRTime > IR_TIMEOUT && lastIRTime != 0)
    {
        this->statusCommand = IR_STOP;  // Vuelve a STOP automáticamente
    }
}
```

### 📊 **Comportamiento del Sistema**

| Acción del Usuario       | Comportamiento del Sistema                          |
| ------------------------ | --------------------------------------------------- |
| **Presionas FORWARD**    | Carro avanza y mantiene dirección                   |
| **Mantienes presionado** | Carro sigue avanzando (recibe señales continuas)    |
| **Suelta el botón**      | Después de 150ms → Carro se detiene automáticamente |
| **Presionas otro botón** | Cambia inmediatamente al nuevo comando              |

### ⚙️ **¿Por Qué Es Necesario el Timeout?**

#### **Limitaciones Técnicas del IR:**

- **Protocolo lento**: ~38kHz vs Bluetooth/WiFi ~2.4GHz
- **No envían señales continuas** mientras mantienes presionado
- **Interferencias** de luz ambiental
- **Ángulo de recepción** limitado

#### **Ventajas del Timeout:**

- ✅ **Seguridad**: Evita que el carro se escape
- ✅ **Control suave**: Movimientos fluidos y naturales
- ✅ **Experiencia de usuario**: Intuitivo y fácil de usar
- ✅ **Eficiencia energética**: Detiene motores cuando no se usa

## 🏗️ Arquitectura del Sistema

### 📁 **Estructura del Proyecto**

```
SmartCarATMEGA328P/
├── 📄 README.md                    # Esta documentación
├── 📂 src/
│   └── 📄 main.cpp                 # Programa principal
├── 📂 lib/
│   ├── 📂 IrControl/              # 🧩 PIEZA CLAVE
│   │   ├── 📄 IrControl.h         # Interfaz del control IR
│   │   └── 📄 IrControl.cpp       # Implementación del sistema
│   ├── 📂 Motor/                  # Control de motores
│   ├── 📂 MPU/                    # Sensor de orientación
│   └── 📂 ...                     # Otras librerías
└── 📄 platformio.ini              # Configuración del proyecto
```

### 🔌 **Componentes Principales**

| Componente             | Función                    | Estado          |
| ---------------------- | -------------------------- | --------------- |
| **ATMEGA328P**         | Microcontrolador principal | ✅ Implementado |
| **Sensor IR**          | Recepción de comandos      | ✅ Implementado |
| **Motores DC**         | Propulsión del carro       | ✅ Implementado |
| **Sistema de Timeout** | Control inteligente        | ✅ Implementado |

## 🚀 Instalación y Uso

### **1. Clonar el Repositorio**

```bash
git clone https://github.com/yourusername/SmartCarATMEGA328P.git
cd SmartCarATMEGA328P
```

### **2. Compilar y Subir**

```bash
# Compilar el proyecto
pio run

# Subir al Arduino
pio run --target upload

# Monitorear Serial
pio device monitor
```

### **3. Control del SmartCar**

#### **Comandos IR Configurados:**

| Botón        | Código IR    | Acción          |
| ------------ | ------------ | --------------- |
| **FORWARD**  | `0xBF40FF00` | Avanzar         |
| **BACKWARD** | `0xE619FF00` | Retroceder      |
| **LEFT**     | `0xF807FF00` | Girar izquierda |
| **RIGHT**    | `0xF609FF00` | Girar derecha   |

#### **Comportamiento Esperado:**

```
🎮 Presionas FORWARD → 🚗 Carro avanza
⏰ Mantienes presionado → 🚗 Sigue avanzando
🛑 Sueltas el botón → 🚗 Se detiene en 150ms
```

## 🔧 Configuración Avanzada

### **Ajuste del Timeout**

```cpp
// En lib/IrControl/IrControl.h
const unsigned long IR_TIMEOUT = 150; // 150ms (ajustable)
```

| Valor   | Comportamiento | Uso Recomendado  |
| ------- | -------------- | ---------------- |
| `50ms`  | Muy sensible   | Control preciso  |
| `150ms` | Balanceado     | **Recomendado**  |
| `300ms` | Menos sensible | Control relajado |
| `500ms` | Muy lento      | No recomendado   |

### **Debug del Sistema**

```cpp
// Habilitar debug en IrControl.cpp
Serial.println("MOVE_FORWARD");  // Comandos reconocidos
Serial.print("Código no reconocido: 0x");  // Códigos desconocidos
```

## 🎯 **¿Por Qué Esta Implementación Es Especial?**

### **1. Resuelve un Problema Real**

- **No es solo código**: Soluciona una limitación física del IR
- **Mejora la UX**: Control intuitivo y natural
- **Mantiene la seguridad**: Timeout automático

### **2. Arquitectura Robusta**

- **Separación de responsabilidades**: IR, motores, lógica separados
- **Fácil mantenimiento**: Código modular y bien documentado
- **Extensible**: Fácil agregar nuevos comandos

### **3. Optimización de Recursos**

- **Bajo consumo**: Timeout evita motores corriendo innecesariamente
- **Respuesta rápida**: 150ms es imperceptible para el usuario
- **Estable**: Manejo robusto de interferencias

## 🤝 Contribuciones

¡Las contribuciones son bienvenidas! Este proyecto es especialmente útil para:

- **Educadores**: Enseñar control de robots
- **Hobbistas**: Proyectos de robótica móvil
- **Estudiantes**: Aprender sistemas embebidos
- **Desarrolladores**: Implementar control IR inteligente

## 📄 Licencia

Este proyecto está bajo la Licencia MIT - ver el archivo [LICENSE](LICENSE) para más detalles.

---

<div align="center">
  <sub>🚗 Built with ❤️ for Smart Robotics | El control IR inteligente que hace la diferencia</sub>
</div>
