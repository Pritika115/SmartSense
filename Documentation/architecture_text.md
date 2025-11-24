# System Architecture - SmartSense

## High-Level Architecture

```
┌─────────────┐
│    USER     │
│  (Student)  │
└──────┬──────┘
       │ Interacts via menu
       ↓
┌─────────────────────────────────┐
│   PYTHON CONSOLE UI             │
│   (smarthome_ui.py)             │
│                                 │
│  - Display telemetry (MQTT)     │
│  - Send commands (REST API)     │
│  - Store data to CSV            │
└────────┬────────────────┬───────┘
         │                │
         │ MQTT           │ REST API
         │ Telemetry      │ Commands
         │ (Temp/Humid)   │ (LED control)
         ↓                ↓
┌─────────────────────────────────┐
│   SERIAL COMMUNICATION          │
│   (USB, 9600 baud)              │
└────────┬────────────────┬───────┘
         │                │
         ↑                ↓
┌─────────────────────────────────┐
│   ARDUINO UNO                   │
│   (smarthome_checkin2.ino)      │
│                                 │
│  - Read DHT22 every 2s          │
│  - Send JSON telemetry          │
│  - Parse LED commands           │
│  - Control LED Pin 13           │
└─────┬────────────┬──────────────┘
      │            │
      ↓            ↓
┌──────────┐  ┌─────────┐
│  DHT22   │  │   LED   │
│  Sensor  │  │ Pin 13  │
│          │  │         │
│ Temp &   │  │ ON/OFF  │
│ Humidity │  │         │
└──────────┘  └─────────┘
```

## Data Flow - MQTT Telemetry

```
DHT22 → Arduino reads → Format JSON → Serial USB → Python UI → Display + CSV
  (every 2 seconds)
```

## Data Flow - REST API Commands

```
User → Python UI → Serial USB → Arduino → LED → Response → Python UI → Display
       (menu)      LED:ON/OFF    (parse)  (control)  (JSON)
```

## API Mapping

| API Type | Purpose | Direction | Hardware |
|----------|---------|-----------|----------|
| MQTT | Continuous telemetry | Arduino → UI | DHT22 |
| REST | Command/Response | UI ↔ Arduino | LED |

