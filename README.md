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

## 🛠️ Hardware / Bill of Material

| Component | Description | Link |
|-----------|-------------|------|
| 🧠 RP2040 Zero | Waveshare or compatible RP2040 Zero devboard | [AliExpress](https://fr.aliexpress.com/item/1005010706978212.html) |
| 🎮 Controller port interface | Mega Drive DB9 connector to direct wire | [AliExpress](https://fr.aliexpress.com/item/1005010279134775.html) |
| ⚡ Level shifting | Required depending on PCB design | [AliExpress](https://fr.aliexpress.com/item/1005007123530432.html) |
| 🟢🔵🟡🟣 Custom PCB | Choose your color! | — |
| 📦 optionnal 3D Case | generic case 26-60-16 | [AliExpress](https://fr.aliexpress.com/item/1005006900224809.html?) |

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

- Pick the right board for your target device 🎯
- Solder the required components — nothing too scary, promise! 🔧
- Flash the correct `.uf2` firmware onto your RP2040 (hold **BOOTSEL** while plugging in USB) ⚡
- Gentlemen, start your Mega Drive / Genesis! 🔥

  ---
  
## 📄 License

_To be defined._

---

## 🙏 Thanks

- [Sik](https://plutiedev.com/)  for his very usefull website , spend a lot of time in the Peripherals part.
- [Ichigobankai](https://www.2minds.fr/fr/)  for agreeing to test and port the code for Master System support (he doesn't know it yet).

---

## 🤝 Contributing

Contributions, issues and feature requests are welcome!  
Feel free to open an [issue](https://github.com/X-death25/MD-IO/issues) or submit a pull request.
