# 🎥 Guion Video – Bloque 2.2: Motores de corriente continua (DC)

## 1. Introducción
- Breve saludo.  
- Explicar que en este video vamos a aprender a fondo cómo funcionan los **motores DC con escobillas** del coche.  
- Resaltar que son el elemento que convierte la energía eléctrica en movimiento.  

---

## 2. ¿Cómo funciona un motor DC?
- Explicar con un esquema sencillo:  
  - Bobina + imán + escobillas.  
  - Al circular corriente → se genera un campo magnético → el rotor gira.  
- Diferencia con otros motores (sin profundizar demasiado):  
  - Brushless (sin escobillas).  
  - Paso a paso.  
- Aquí nos centraremos en el **motor DC con escobillas**, por ser el que usa el Elegoo Smart Car.  

---

## 3. Relación de engranajes
- Mostrar que el motor del coche no está “solo”, sino acoplado a un **sistema de engranajes**.  
- ¿Por qué?  
  - Para aumentar el torque (fuerza).  
  - Para reducir la velocidad a una más adecuada para el robot.  
- Ejemplo práctico: *“Sin engranajes, el motor giraría demasiado rápido pero con muy poca fuerza. Los engranajes equilibran esto.”*  

---

## 4. Control a nivel hardware: el puente H
- Explicar qué es un **puente H**:  
  - Es un circuito que permite controlar la dirección de giro del motor (adelante/atrás).  
- Señalar que en la placa del coche ya viene integrado.  
- Relación con el Arduino:  
  - El Arduino manda señales lógicas.  
  - El puente H las traduce en potencia para mover el motor.  

---

## 5. Control de velocidad con PWM
- Explicar de forma sencilla qué es el **PWM (Pulse Width Modulation)**:  
  - Señales encendido/apagado muy rápidas.  
  - Al variar el “duty cycle” (ciclo de trabajo), se regula la velocidad del motor.  
- Ejemplo cotidiano: *“Es como encender y apagar una bombilla muy rápido: si está más tiempo encendida que apagada, parece más brillante. Con el motor pasa igual: más tiempo encendido → más rápido.”*  

---

## 6. Práctica: calibración del motor
- Revisar el **datasheet** del motor:  
  - Voltaje nominal.  
  - Corriente en vacío.  
  - Corriente máxima.  
  - Velocidad nominal (RPM).  
- Explicar la importancia de **caracterizar un motor antes de usarlo** (buena práctica de ingeniería).  
- Hacer una prueba de calibración:  
  - Encenderlo a distintas señales PWM.  
  - Observar diferencias en velocidad, fuerza y consumo.  

👉 Objetivo: **desarrollar pensamiento crítico y capacidad de diseño**.  

---

## 7. Cierre
- Resumir lo aprendido:  
  - Funcionamiento interno del motor DC con escobillas.  
  - Importancia de los engranajes.  
  - Control mediante puente H y PWM.  
  - Revisión de datasheet y calibración básica.  
- Adelantar: en el próximo video veremos otro sensor/actuador del coche.  
- Despedida breve.  
