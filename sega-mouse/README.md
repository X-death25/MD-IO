# Sega Mouse Adapter (RP2040)

> Use **any standard USB mouse** as a Sega Mouse on your Mega Drive / Mega-CD — no need to hunt down a rare original Mega Mouse!

---

## 🧠 Overview

This module turns a cheap, generic USB mouse into a fully compatible **Sega Mouse (Mega Mouse)** replacement for the Sega Mega Drive and Mega-CD.

- **Core 0** emulates the native Sega Mouse protocol (TH/TR/TL handshake, 9-nibble packet) directly on the DB9 controller port
- **Core 1** reads the USB mouse via USB Host (TinyUSB)

The most cheap usb mouse will me enough , no need to use RGB gamer mouse ;)
---

## 🎮 Compatibility

Tested on real hardware (console + flashcart + original cartridges/CDs).

<p align="center">
  <img src="images/monkey-island.jpg" width="45%" alt="The Secret of Monkey Island running with mouse support"/>
  <img src="images/dungeon-master-2.jpg" width="45%" alt="Dungeon Master II running with mouse support"/>
</p>

> ⚠️ **Port matters!** Not all games expect the mouse on the same controller port. Some titles look for it on **Port 1**, others on **Port 2** — plug it into the wrong port and the game will simply fall back to pad input (or not respond at all), even though the adapter itself is working fine. Check the port column in the full compatibility file below before assuming a game is "not working."

Full details (including the exact port used per game) are tracked in [`compat.xlsx`](./compat.xlsx) — the tables below are a quick summary only.

### Mega Drive

| Game | Status | Region |
|---|---|---|
| Marble Madness | ✅ Works good | Japan |
| Nobunaga's Ambition | ✅ Works good | USA |
| Two Tribes - Populous II | ✅ Works good | Europe |
| Janou Touryuumon | ✅ Works good | Japan |
| Fun 'n' Games | ✅ Works good | USA |
| Shanghai II: Dragon's Eye | ✅ Works good | USA |
| Lord Monarch: Tokoton Sentou Densetsu | ✅ Works good | Japan |
| Body Count | ✅ Works good | Europe |
| Jimmy White's Whirlwind Snooker | ✅ Works good | Europe |
| Lemmings 2: The Tribes | ✅ Works good | USA |
| Richard Scarry's Busytown | ✅ Works good | USA |
| Wacky Worlds | ✅ Works good | USA |
| Cannon Fodder | ⚠️ Not Working — buggy mouse routine, see [Known limitations](#️-known-limitations) | Europe |
| Lizard Paint | ✅ Works good | Region Free |
| Wolfenstein 3D by gasega68k | ✅ Works good | Region Free |
| Dune (+ mouse hack) | ✅ Works good | USA |

### Mega-CD

| Game | Status | Region |
|---|---|---|
| The Secret of Monkey Island | ✅ Works good | EUR |
| Dungeon Master II | ✅ Works good | USA |
| Eye of the Beholder | ✅ Works good | USA |
| Mansion of the Hidden Souls | ✅ Works good | EUR |
| My Paint | ✅ Works good | USA |
| Panic | ✅ Works good | USA |
| Urusei Yatsura: Dear My Friends | ❌ Not Working | JAP |
| Warau Salesman | ✅ Works good | JAP |
| Yumemi Mystery Mansion | ✅ Works good | JAP |
| Nobunaga no Yabou | ✅ Works good | JAP |
| Shin Megami Tensei | ✅ Works good | JAP |
| StarBlade | ⚠️ Non-standard mouse routine, see [Known limitations](#️-known-limitations) | — |

---

## ⭐ Recommended games

A few highlights worth trying first if you're not sure where to start:

| Game | Why |
|---|---|
| **The Secret of Monkey Island** | Point & click classic — mouse control feels exactly like the PC original |
| **Lemmings 2: The Tribes** | The mouse genuinely changes how playable this is compared to pad controls |
| **Wolf 3d** | Awesome PC Wolf 3D port by GASEGA68k support Sega mouse very well  |
| **Dungeon Master II: Skullkeep** | Full mouse-driven dungeon crawler, great showcase of precise cursor control |
| **Shanghai II** | Very cute and nice Puzzle game for a chill session  |
| **Dune The Battle For Arrakis ** | ( mouse hack => (https://www.romhacking.net/hacks/6403/)) Mouse support via the well-known "mouse hack" — a fun curiosity for RTS fans |
| **Eye of the Beholder** | Another dungeon crawler classic, mouse-first UI |
| **Lord Monarch: Tokoton Sentou Densetsu** | Real-time castle-vs-castle strategy from Falcom's Dragon Slayer series — got an English translation patch, finally accessible to non-Japanese speakers |

---

## ⚠️ Known limitations

- **Urusei Yatsura: Dear My Friends (JAP)** — desyncs after a few nibbles; the game's own mouse polling appears fragile. Not yet resolved.

- **Cannon Fodder** — has a known-buggy mouse acquisition routine that only works on real Sega Mouse "by chance" (according to Eke Eke), relying on the natural ACK delay between a TR change and the corresponding TL response.
Emulators that answer TR instantaneously hit the same issue. Documented by [Eke on the Genesis dev forum](https://gendev.spritesmind.net/forum/memberlist.php?mode=viewprofile&u=41&sid=a11b0d1857255a04f9681187571375ca). Interestingly, **Shanghai II** — cited in the same thread as having an equally buggy routine — works perfectly with my adapter... Maybe because the BlastEM-derived timings used here happen to land in the right window. (another thanks to Emu dev who found the way long years ago...).
- **StarBlade (Mega-CD)** — uses a non-standard mouse acquisition routine, different from the usual 9-nibble handshake. According to Eke, it breaks if the emulator/adapter applies a uniform TL delay on the 0x40→0x60 transition 
— the opposite fix from what Cannon Fodder needs. Not currently supported.

---

## 💻 SGDK demo

A minimal SGDK sample (`/sgdk`) demonstrating mouse cursor movement and click detection — useful as a reference implementation for your own homebrew.

---

## 🛠️ Firmware

See [`/firmware`](./firmware) for the RP2040 uf2 file.

---

## 📝 Additional Notes

### Saturn Shuttle Mouse compatibility

Since the Sega Saturn I/O port shares the exact same signal lines as the Mega Drive port (just a different connector shape), a **passive adapter** (simple rewiring, no active components) is enough to use a genuine **Saturn Shuttle Mouse** on your Mega Drive/Mega-CD — no RP2040 needed for that specific case, since the Shuttle Mouse already speaks the native Sega Mouse protocol.

<p align="center">
  <img src="images/saturn-pinout.png" width="70%" alt="Saturn I/O port pinout by Plutiedev"/>
</p>

| Common name | Mega Drive style name |
|---|---|
| VCC | VCC |
| GND | GND |
| D0 | DATA0 |
| D1 | DATA1 |
| D2 | DATA2 |
| D3 | DATA3 |
| S0 | TH |
| S1 | TR |
| +5V | TL |

> Pinout diagram credit: [Sik / Plutiedev](https://plutiedev.com/) (public domain)

Saturn Shuttle mouse seems to works fine even in the non working game in my compatibility list.

### Why this project exists

Genuine Sega/Mega Mouse units are rare and often expensive on the second-hand market. This adapter lets you use any cheap USB mouse instead, while still being *near* fully compatible with the original protocol — so it also works as a drop-in replacement if you don't want to risk your only genuine unit.

Beyond that, this project is also about **preservation**: a lot of time went into probing the real protocol with a logic analyzer, comparing it against a genuine Saturn Shuttle Mouse, and documenting the quirks and edge cases along the way (see [`/docs`](./docs)). The goal is to leave behind an accurate, tested reference — both for keeping these old peripherals usable long-term, and for anyone writing new SGDK homebrew who wants proper mouse support without having to buy an expensive and rare original mouse.

> 🔜 A **Saturn Keyboard** adapter is next on the list — stay tuned!

---

## 📓 Docs

Technical notes, protocol traces, and debugging logs from development are kept in [`/docs`](./docs) — useful if you're reverse-engineering similar peripherals or want to understand the TH/TR/TL handshake in detail.