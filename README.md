# MD-IO

> **Work In Progress** — USB peripherals on Sega Mega Drive, powered by RP2040 Zero

<p align="center">
  <img src="https://github.com/X-death25/MD-IO/blob/main/images/logo.png" width="50%" alt="MD-IO Logo"/>
</p>

---

## 🧠 Overview

**MD-IO** is an open hardware & software project that enables modern USB peripherals to interface with the **Sega Mega Drive / Genesis** controller port.

Powered by the **RP2040 Zero** in USB Host mode, MD-IO acts as a bridge between USB devices and the Mega Drive input protocol.
One core is used for USB Host.
Other core is used for Megadrive controller.
Goal of this project is to use some cool USB device with my megadrive like Astro city gamepad , arcade stick and preserve some rarely device just like mouse , keyboard or maybe printer ;)

---

## 🛠️ Hardware

| Component | Description | Link |
|-----------|-------------|
| RP2040 Zero | Dual Core RP with native USB Host support |
| Controller port interface | Mega Drive DB9 connector wiring |
| Level shifting | Required depending on PCB design |
| Custom PCB | use correct color PCB wanted device |

### 🔍 Planned Hardware

| Module | Description |
|--------|-------------|
| 3 bouton controller | Classic Sega Gamepad can also be used in Master System / Mark 3 |
| 6 bouton controller | Extended 6 boutons Sega Gamepad with mode support for legacy use|
| Sega Mouse | 3 boutons Sega Mouse|
| Saturn Keyboard | Saturn keyboard can be used in MD with a passive adaptater so need to be suported here too ;)|
| Printer| -_-' yes , i have some idea|
| 📡 Open-source probe | Debug & signal analysis tool |
| 🧾 Serial logger | Peripheral for debugging Mega Drive software : send direct log from your MD to PC |

---

## 💾 Supported Peripherals & Firmware

| Peripheral | Type | Firmware | Status |
|------------|------|----------|--------|
| — | — | — | 🚧 Coming soon |

> This table will be updated as peripherals are tested and firmware is released.

---

## 💻 Software

- Written in **C/C++** using the [Pico SDK](https://github.com/raspberrypi/pico-sdk)
- USB Host stack integration
- Device abstraction layer for peripherals
- Mega Drive protocol implementation

---

## 🚀 Getting Started

### Requirements

- Raspberry Pi Pico or any RP2040-based board
- USB peripherals (mouse, keyboard, gamepad, etc.)
- Sega Mega Drive / Genesis console

### Flash

> Flash the `.uf2` file to your RP2040 board by holding BOOTSEL while plugging in USB.

---

## 📄 License

_To be defined._

## 🙏 Thanks

[Sik ](https://plutiedev.com/)

---

## 🤝 Contributing

Contributions, issues and feature requests are welcome!
Feel free to open an [issue](https://github.com/X-death25/MD-IO/issues) or submit a pull request.
