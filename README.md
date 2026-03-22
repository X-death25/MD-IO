# MD-IO

> **Work In Progress** — USB peripherals on Sega Mega Drive, powered by RP2040 Zero

<p align="center">
  <img src="https://github.com/X-death25/MD-IO/blob/main/images/logo.png" width="50%" alt="MD-IO Logo"/>
</p>

---

## 🧠 Overview

**MD-IO** is an open hardware & software project that enables modern USB peripherals to interface with the **Sega Mega Drive / Genesis** controller port.

Powered by the **RP2040 Zero** in USB Host mode, MD-IO acts as a bridge between USB devices and the Mega Drive input protocol.

- One core is dedicated to **USB Host**
- The other core handles the **Mega Drive controller protocol**

The goal of this project is to use cool USB devices with my Mega Drive — like the Astro City gamepad, arcade sticks — and to preserve some rare peripherals like the mouse, keyboard, or maybe even a printer 😄

---

## 🛠️ Hardware

| Component | Description | Link |
|-----------|-------------|------|
| 🧠 RP2040 Zero | Waveshare or compatible RP2040 Zero devboard | [AliExpress](https://fr.aliexpress.com/item/1005010706978212.html) |
| 🎮 Controller port interface | Mega Drive DB9 connector wiring | [AliExpress](https://fr.aliexpress.com/item/1005010279134775.html) |
| ⚡ Level shifting | Required depending on PCB design | [AliExpress](https://fr.aliexpress.com/item/1005007123530432.html) |
| 🟢🔵🟡🟣 Custom PCB | Choose your color! | — |

### 🔍 Planned Hardware

| Module | Description |
|--------|-------------|
| 🕹️ 3-button controller | Classic Sega gamepad — also compatible with Master System / Mark III |
| 🕹️ 6-button controller | Extended Sega gamepad with Mode button support for legacy use |
| 🖱️ Sega Mouse | 3-button Sega Mouse |
| ⌨️ Saturn Keyboard | Works on MD via a passive adapter — needs to be supported here too 😉 |
| 🖨️ Printer | Yes. I have ideas. |
| 📡 Open-source probe | Debug & signal analysis tool |
| 🧾 Serial logger | Send logs directly from your MD to PC |

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

- RP2040 Zero (or any RP2040-based board)
- USB peripherals (mouse, keyboard, gamepad, etc.)
- Sega Mega Drive / Genesis console

### Flash

> Flash the `.uf2` file to your RP2040 board by holding **BOOTSEL** while plugging in USB.

---

## 📄 License

_To be defined._

---

## 🙏 Thanks

- [Sik](https://plutiedev.com/)

---

## 🤝 Contributing

Contributions, issues and feature requests are welcome!  
Feel free to open an [issue](https://github.com/X-death25/MD-IO/issues) or submit a pull request.
