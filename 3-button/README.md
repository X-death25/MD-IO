# 3-Button Controller Adapter (RP2040)

> Use a standard USB gamepad as a classic Sega 3-button controller on your Mega Drive / Master System.

## 🧠 Overview

Straightforward USB-to-DB9 passthrough for the base 3-button Sega pad protocol — no special handshake quirks, just standard controller polling.

## 🛠️ Firmware

Pick the `.uf2` matching your USB gamepad from [`/firmware`](./firmware):

| Gamepad | Firmware |
|---|---|
| Mega Drive Mini USB gamepad | `md-mini-3button.uf2` |
| AliExpress NES-style USB gamepad | `nes-usb-aliexpress.uf2` *(coming soon)* |

> 📌 Same closed-source-for-now policy as the other modules in this repo.