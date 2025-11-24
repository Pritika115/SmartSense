# Wiring Diagram - SmartSense

## Components Connection

```
Arduino Uno
    │
    ├─── Pin 2 ────→ DHT22 DATA (middle pin)
    │
    ├─── 5V ───────→ DHT22 VCC (+ pin)
    │
    ├─── GND ──────→ DHT22 GND (- pin)
    │
    ├─── Pin 13 ───→ 220Ω Resistor ───→ LED (+) long leg
    │
    └─── GND ──────→ LED (-) short leg
```

## DHT22 (AM2302) Sensor
- **VCC (+)** → Arduino 5V
- **DATA (out)** → Arduino Pin 2
- **GND (-)** → Arduino GND

## LED Circuit
- **Anode (+) long leg** → 220Ω Resistor → Arduino Pin 13
- **Cathode (-) short leg** → Arduino GND

## Visual Layout

```
        DHT22 Sensor              Arduino Uno              LED
        ┌─────────┐               ┌─────────┐
        │  + o S  │               │  Pin 2  │◄───── DATA
        │  AM2302 │               │  5V     │◄───── VCC
        │    -    │               │  GND    │◄───── GND
        └─────────┘               │  Pin 13 │─────►[220Ω]─►[>|]──► GND
                                  │         │
                                  └─────────┘
```
