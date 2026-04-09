# 🚗 Voice-Controlled In-Vehicle System

> **Electronics Engineering Project — Year 3, Term 2**

A voice-controlled in-vehicle infotainment and control system built with Arduino, CAN Bus communication (MCP2515), and a TFT display. The system allows drivers to interact with in-car functions hands-free through voice commands, with data transmitted between nodes via the CAN protocol.

---

## 📋 Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Hardware Requirements](#hardware-requirements)
- [System Architecture](#system-architecture)
- [Repository Structure](#repository-structure)
- [Getting Started](#getting-started)
- [CAN Bus Configuration](#can-bus-configuration)
- [Dependencies & Libraries](#dependencies--libraries)
- [Authors](#authors)

---

## Overview

This project implements a voice-controlled interface for an in-vehicle system. A microcontroller receives voice commands, processes them, and transmits control signals over a CAN Bus network to peripheral nodes (e.g., display unit, actuator controllers). The system mirrors how modern automotive ECUs communicate — reliably and in real time.

---

## Features

- 🎙️ **Voice Command Recognition** — Hands-free control of in-vehicle functions
- 🖥️ **TFT Display Output** — Visual feedback rendered on a TFT screen
- 🔗 **CAN Bus Communication** — Inter-node messaging via MCP2515 CAN controller (CAN 2.0B)
- ⚡ **Real-Time Data Transmission** — Low-latency message passing between system nodes
- 🔧 **Modular Node Design** — Separate transmitter and receiver nodes connected via CAN_H / CAN_L

---

## Hardware Requirements

| Component | Description |
|---|---|
| Arduino (Uno / Mega) | Main microcontroller for each node |
| MCP2515 Module | CAN Bus controller with TJA1050 transceiver |
| TFT Display | Visual output screen (ILI9341 or compatible) |
| Microphone / Voice Module | Voice input capture |
| Jumper Wires & Breadboard | Prototyping connections |
| 120Ω Resistor | CAN Bus line termination (one per bus end) |

---

## System Architecture

```
┌─────────────────────┐         CAN Bus          ┌─────────────────────┐
│   Voice Input Node  │  ───── CAN_H / CAN_L ─── │ Display/Output Node │
│                     │                          │                     │
│  Microphone         │                          │  TFT Display        │
│                     │                          │                     │
│  MCP2515 (TX)       │                          │  Sn65hvd230 (RX)    │
└─────────────────────┘                          └─────────────────────┘
```

- **Node 1 (Transmitter):** Captures and processes voice commands, encodes them as CAN frames, and transmits over the bus.
- **Node 2 (Receiver):** Receives CAN frames, decodes commands, and updates the TFT display or triggers corresponding actions.

---

## Repository Structure

```
Voice-Controlled-In-Vehicle-System/
├── Code/               # Arduino source code for all nodes
├── Document/           # Project documentation, reports, schematics
├── .vscode/            # VS Code workspace settings
├── .gitignore
└── Readme.md
```

---

## Getting Started

### 1. Clone the Repository

```bash
git clone https://github.com/EARTHISALWAYSHAPPY/Voice-Controlled-In-Vehicle-System.git
cd Voice-Controlled-In-Vehicle-System
```

### 2. Install Required Libraries

In the Arduino IDE, go to **Sketch → Include Library → Manage Libraries** and install:

- `mcp2515` — by autowp (CAN Bus controller)
- `TFT_eSPI` or `Adafruit GFX` — for TFT display

### 3. Wire the Hardware

**MCP2515 → Arduino SPI Wiring:**

| MCP2515 Pin | Arduino Pin |
|---|---|
| VCC | 5V |
| GND | GND |
| CS | 10 |
| MOSI | 11 (MOSI) |
| MISO | 12 (MISO) |
| SCK | 13 (SCK) |
| INT | 2 |

Connect **CAN_H** and **CAN_L** between both MCP2515 modules. Add a **120Ω termination resistor** across CAN_H and CAN_L on one end of the bus.

### 4. Upload the Code

- Open the transmitter sketch from `Code/` and upload to **Node 1 (Arduino)**
- Open the receiver sketch from `Code/` and upload to **Node 2 (Arduino)**
- Open Serial Monitor at the appropriate baud rate to observe communication

---

## CAN Bus Configuration

The MCP2515 is configured to communicate using the CAN 2.0B standard:

```c
mcp2515.reset();
mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ);
mcp2515.setNormalMode();
```

CAN frames carry encoded voice command data in the `data[]` payload field and are identified by a unique `can_id` per command type.

---

## Dependencies & Libraries

| Library | Purpose | Source |
|---|---|---|
| `arduino-mcp2515` | CAN Bus controller driver | [autowp/arduino-mcp2515](https://github.com/autowp/arduino-mcp2515) |
| `TFT_eSPI` | TFT display rendering | [Bodmer/TFT_eSPI](https://github.com/Bodmer/TFT_eSPI) |
| `SPI.h` | SPI communication (built-in) | Arduino Standard Library |

---

## Authors

Developed as part of the **Electronics Engineering Year 3 Term 2** curriculum.

- [@EARTHISALWAYSHAPPY](https://github.com/EARTHISALWAYSHAPPY)

---

## License

This project is for educational purposes. Please refer to the repository for any licensing details.