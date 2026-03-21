# MD-IO

> **Work In Progress** — USB peripherals on Sega Mega Drive, powered by RP2040

<p align="center">
  <img src="https://github.com/X-death25/MD-IO/blob/main/images/logo.png" width="50%" alt="MD-IO Logo"/>
</p>

---

## 🧠 Overview

**MD-IO** is an open hardware & software project that enables modern USB peripherals to interface with the **Sega Mega Drive / Genesis** controller port.

Powered by the **RP2040** in USB Host mode, MD-IO acts as a bridge between USB devices and the Mega Drive input protocol.

---

## 🛠️ Hardware

| Component | Description |
|-----------|-------------|
| RP2040 | Microcontroller with native USB Host support |
| Controller port interface | Mega Drive DB9 connector wiring |
| Level shifting | Required depending on PCB design |
| Custom PCB | Optional — reference design available |

### 🔍 Planned Hardware

| Module | Description |
|--------|-------------|
| 📡 Open-source probe | Debug & signal analysis tool |
| 🧾 Serial logger | Peripheral for debugging Mega Drive software |

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
