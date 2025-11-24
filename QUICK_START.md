# SmartSense - Quick Start Guide for Check-In 2

## 🚀 Quick Setup (10 Minutes)

### Step 1: Hardware (5 minutes)
1. Connect DHT11 to Arduino:
   - Red wire: VCC → 5V
   - Yellow wire: DATA → Pin 2
   - Black wire: GND → GND

2. Connect LED to Arduino:
   - LED long leg → 220Ω resistor → Pin 13
   - LED short leg → GND

3. Connect Arduino to computer via USB

### Step 2: Arduino Software (3 minutes)
1. Open Arduino IDE
2. Install DHT library: Tools → Manage Libraries → Search "DHT sensor library by Adafruit"
3. Open: `Arduino/smarthome_checkin2.ino`
4. Select: Tools → Board → Arduino Uno
5. Select: Tools → Port → (your COM port)
6. Click Upload ➜ button
7. **Note your COM port number!**

### Step 3: Python Software (2 minutes)
1. Open terminal/command prompt
2. Install pyserial:
   ```bash
   pip install pyserial
   ```
3. Run UI:
   ```bash
   cd "c:\Users\Yuk1N0\OneDrive\Desktop\homework\API\smarthome"
   python UI/smarthome_ui.py
   ```
4. Enter your COM port (e.g., COM3)

---

## ✅ Quick Test

Once running, you should see:
1. ✓ Temperature data streaming every 2 seconds (MQTT)
2. ✓ Menu with 5 options
3. ✓ LED turns on when you select option 1
4. ✓ LED turns off when you select option 2

---

## 🎯 For Check-In 2 Demo

### What to Show:
1. **Hardware:** Point out DHT11 sensor and LED
2. **Telemetry:** Show temperature updates (MQTT-style)
3. **Control:** Turn LED on/off from UI (REST API-style)
4. **Data:** Show sensor_data.csv file

### What to Explain:
- **MQTT:** Continuous temperature streaming from Arduino to UI
- **REST API:** Command-response for LED control from UI to Arduino
- **Two hardware:** DHT11 (input) and LED (output)
- **Data storage:** CSV file with timestamps

---

## 📊 Example Output

```
SmartSense Control Panel
==================================================
1. Turn LED ON (REST API)
2. Turn LED OFF (REST API)
3. Get System Status (REST API)
4. View Recent Data Summary
5. Exit
==================================================

Note: Temperature data streams automatically (MQTT)

[TELEMETRY] Temp: 23.5°C | Humidity: 45.2% | Device: arduino_uno
[TELEMETRY] Temp: 23.6°C | Humidity: 45.1% | Device: arduino_uno
```

---

## 🔧 Common Issues

| Problem | Solution |
|---------|----------|
| Can't connect | Close Arduino IDE Serial Monitor |
| No temperature data | Check DHT11 wiring (especially Pin 2) |
| LED doesn't work | Check polarity (long leg = positive) |
| Module not found | Run: `pip install pyserial` |

---

## 📁 Files You Created

```
smarthome/
├── Arduino/
│   └── smarthome_checkin2.ino      ← Arduino code
├── UI/
│   └── smarthome_ui.py             ← Python UI
├── sensor_data.csv                  ← Generated data
├── SETUP_INSTRUCTIONS.md            ← Detailed setup
├── ARCHITECTURE_GUIDE.md            ← Diagram guide
└── QUICK_START.md                   ← This file
```

---

## 📝 Before Your Meeting

□ Test system end-to-end
□ Create architecture diagram (see ARCHITECTURE_GUIDE.md)
□ Update README.md with Check-In 2 sections
□ Ensure each team member has 4+ commits
□ Prepare to explain MQTT vs REST API usage

---

## 🎓 Key Concepts for Discussion

**MQTT Simulation:**
- Continuous, one-way data streaming
- Temperature published every 2 seconds
- No request needed, data pushed automatically

**REST API Simulation:**
- Request-response pattern
- User requests action (LED:ON)
- Arduino responds with status

**Why Serial?**
For Check-In 2, we're simulating MQTT and REST over serial to prove the concept. For final submission, you'll implement actual MQTT broker and REST server with WiFi.

---

Good luck! 🎉
