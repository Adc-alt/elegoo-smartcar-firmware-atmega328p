# 🐱 Smart Pet Feeder / Alimentador Automático para Mascotas

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![ESP8266](https://img.shields.io/badge/ESP8266-NodeMCU-orange)](https://www.espressif.com/en/products/socs/esp8266)
[![PlatformIO](https://img.shields.io/badge/PlatformIO-IDE-blue)](https://platformio.org/)

<div align="center">
  <img src="images/assembled.jpg" alt="Smart Pet Feeder" width="400"/>
</div>

## 📝 Descripción / Description

Este proyecto implementa un alimentador automático para mascotas utilizando un ESP8266. El sistema permite programar horarios de alimentación y monitorear el estado del dispensador de comida.

This project implements an automatic pet feeder using an ESP8266. The system allows scheduling feeding times and monitoring the food dispenser status.

## ✨ Características / Features

- 🕒 Programación de horarios de alimentación / Feeding schedule programming
- 📱 Monitoreo remoto / Remote monitoring
- 📶 Control por WiFi / WiFi control
- 🔋 Bajo consumo de energía / Low power consumption

## 📁 Estructura del Proyecto / Project Structure

```
├── 📄 README.md               # Project presentation
├── 📂 docs/
│   └── 📄 full_documentation.md  # Extended technical documentation
├── 📂 hardware/
│   ├── 📄 PCB_layout.json     # PCB design (EasyEDA)
│   ├── 📄 schematic.pdf       # Exported schematic
│   ├── 📄 bom.csv            # Bill of materials
│   └── 📂 gerbers/           # Gerber files folder
├── 📂 firmware/              # ESP8266 code
│   └── 📄 main.c
├── 📂 images/
│   ├── 📄 pcb_photo.jpg
│   └── 📄 assembled.jpg
```

## 🛠️ Requisitos / Requirements

- 🔌 ESP8266
- ⚙️ Servomotor
- 📊 Sensor de nivel
- 🔌 Fuente de alimentación 5V
- 🏗️ Carcasa impresa en 3D

## 📥 Instalación / Installation

1. 📥 Clonar el repositorio / Clone the repository
   ```bash
   git clone https://github.com/yourusername/smart-pet-feeder.git
   ```
2. 📦 Instalar dependencias / Install dependencies
   ```bash
   cd smart-pet-feeder
   platformio run
   ```
3. ⚡ Compilar y subir el firmware / Compile and upload firmware
   ```bash
   platformio run --target upload
   ```

## 📄 Licencia / License

Este proyecto está bajo la Licencia MIT - ver el archivo [LICENSE](LICENSE) para más detalles.

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

<div align="center">
  <sub>Built with ❤️ by <a href="https://github.com/yourusername">Your Name</a></sub>
</div>
