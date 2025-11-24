# SmartSense - Setup Instructions for Check-In 2

## Table of Contents
1. [Hardware Setup](#hardware-setup)
2. [Software Setup](#software-setup)
3. [Running the System](#running-the-system)
4. [Testing](#testing)
5. [Troubleshooting](#troubleshooting)

---

## Hardware Setup

### Components Required
- Arduino Uno board
- DHT11 Temperature & Humidity Sensor
- LED (any color)
- 220Ω Resistor (for LED)
- Breadboard
- Jumper wires (male-to-male)
- USB A-to-B cable (for Arduino)

### Wiring Diagram

```
DHT11 Sensor Connections:
├─ VCC (Pin 1)  → Arduino 5V
├─ DATA (Pin 2) → Arduino Digital Pin 2
└─ GND (Pin 4)  → Arduino GND

LED Circuit:
├─ LED Anode (+) → 220Ω Resistor → Arduino Digital Pin 13
└─ LED Cathode (-) → Arduino GND
```

### Step-by-Step Wiring Instructions

1. **Place components on breadboard:**
   - Insert DHT11 sensor into breadboard
   - Insert LED into breadboard (note the longer leg is positive)
   - Insert 220Ω resistor into breadboard

2. **Connect DHT11 Sensor:**
   - Connect DHT11 VCC pin to Arduino 5V using red jumper wire
   - Connect DHT11 DATA pin to Arduino Digital Pin 2 using yellow jumper wire
   - Connect DHT11 GND pin to Arduino GND using black jumper wire

3. **Connect LED:**
   - Connect LED positive leg (longer) to one end of 220Ω resistor
   - Connect other end of resistor to Arduino Digital Pin 13 using jumper wire
   - Connect LED negative leg (shorter) to Arduino GND using black jumper wire

4. **Verify connections:**
   - Double-check all connections match the wiring diagram
   - Ensure no short circuits between power and ground
   - Make sure LED polarity is correct (longer leg to resistor)

---

## Software Setup

### 1. Arduino IDE Setup

#### Install Arduino IDE
- Download from: https://www.arduino.cc/en/software
- Install version 2.0 or later
- Run the installer and follow prompts

#### Install DHT Sensor Library
1. Open Arduino IDE
2. Go to: **Tools → Manage Libraries...**
3. Search for "DHT sensor library"
4. Install **"DHT sensor library by Adafruit"**
5. When prompted, also install **"Adafruit Unified Sensor"** library

#### Upload Arduino Code
1. Open Arduino IDE
2. Go to: **File → Open**
3. Navigate to: `Arduino/smarthome_checkin2.ino`
4. Select the file and click **Open**
5. Connect Arduino Uno to computer via USB cable
6. Select board: **Tools → Board → Arduino AVR Boards → Arduino Uno**
7. Select port: **Tools → Port → COMX** (Windows) or **/dev/ttyACMX** (Mac/Linux)
8. Click **Upload** button (right arrow icon)
9. Wait for "Done uploading" message
10. **Important:** Note the COM port number for later use

### 2. Python Setup

#### Install Python
- Download Python 3.8 or later from: https://www.python.org/downloads/
- During installation, check "Add Python to PATH"
- Verify installation: Open terminal/command prompt and type `python --version`

#### Install Required Python Packages
Open terminal/command prompt and run:

```bash
pip install pyserial
```

This installs the `pyserial` library needed for serial communication with Arduino.

#### Verify Python UI Files
Ensure the following file exists:
- `UI/smarthome_ui.py`

---

## Running the System

### Step 1: Upload Arduino Code
1. Open Arduino IDE
2. Open `Arduino/smarthome_checkin2.ino`
3. Connect Arduino via USB
4. Select correct board and port
5. Click Upload
6. **Close Arduino IDE Serial Monitor if open** (important!)

### Step 2: Find Your COM Port

**Windows:**
- Open Device Manager
- Look under "Ports (COM & LPT)"
- Find "Arduino Uno (COMX)" - note the number (e.g., COM3)

**Mac:**
- Open Terminal
- Run: `ls /dev/tty.*`
- Look for `/dev/tty.usbmodemXXXX` or `/dev/tty.usbserialXXXX`

**Linux:**
- Open Terminal
- Run: `ls /dev/ttyACM*` or `ls /dev/ttyUSB*`
- Usually `/dev/ttyACM0`

### Step 3: Run Python UI
1. Open terminal/command prompt
2. Navigate to project directory:
   ```bash
   cd "c:\Users\Yuk1N0\OneDrive\Desktop\homework\API\smarthome"
   ```
3. Run the UI:
   ```bash
   python UI/smarthome_ui.py
   ```
4. When prompted, enter your COM port (e.g., `COM3` or `/dev/ttyACM0`)

### Step 4: Use the System
Once connected, you'll see:
- **Automatic temperature telemetry** streaming every 2 seconds (MQTT-style)
- **Menu options** for LED control (REST API-style)

**Menu Options:**
1. **Turn LED ON** - Sends REST API command to turn on LED
2. **Turn LED OFF** - Sends REST API command to turn off LED
3. **Get System Status** - Requests current LED state via REST API
4. **View Recent Data Summary** - Shows last 5 readings and averages
5. **Exit** - Closes the application

---

## Testing

### Test 1: Verify Telemetry (MQTT Simulation)
**Expected behavior:** Temperature and humidity data appears every 2 seconds

```
[TELEMETRY] Temp: 23.5°C | Humidity: 45.2% | Device: arduino_uno
```

**What to check:**
- Data updates regularly (every 2 seconds)
- Temperature values are reasonable (15-30°C room temp)
- Humidity values are reasonable (30-70% typical)
- Data is saved to `sensor_data.csv`

**If it fails:**
- Check DHT11 wiring (especially DATA pin to Pin 2)
- Verify DHT11 library is installed
- Try blowing on sensor to see if temperature changes

### Test 2: LED ON Command (REST API)
**Steps:**
1. Select option `1` from menu
2. Press Enter

**Expected behavior:**
- LED turns ON physically
- Console shows: `[REST API] ✓ Success: LED_ON`

**If it fails:**
- Check LED wiring (Pin 13, resistor, polarity)
- Verify LED is not burned out (swap with new one)
- Check resistor connection

### Test 3: LED OFF Command (REST API)
**Steps:**
1. Select option `2` from menu
2. Press Enter

**Expected behavior:**
- LED turns OFF physically
- Console shows: `[REST API] ✓ Success: LED_OFF`

### Test 4: System Status Query (REST API)
**Steps:**
1. Turn LED ON (option 1)
2. Select option `3` from menu
3. Press Enter

**Expected behavior:**
- Console shows: `[REST API] ✓ Success: ON`

### Test 5: Data Summary
**Steps:**
1. Let system run for at least 10 seconds (collect 5+ readings)
2. Select option `4` from menu
3. Press Enter

**Expected behavior:**
- Shows last 5 temperature/humidity readings with timestamps
- Shows average temperature and humidity
- Shows total number of readings

### Test 6: Data Persistence
**Steps:**
1. Run system for 30 seconds
2. Exit program (option 5)
3. Open `sensor_data.csv` in Excel or text editor

**Expected behavior:**
- CSV file contains multiple rows of data
- Each row has: Timestamp, Temperature, Humidity, Device
- Data is formatted correctly

---

## Troubleshooting

### Problem: "Connection failed" or "Port not found"
**Solutions:**
- Make sure Arduino is connected via USB
- Close Arduino IDE Serial Monitor
- Try a different USB port
- Check Device Manager (Windows) for correct COM port
- On Mac/Linux, check permissions: `sudo chmod 666 /dev/ttyACM0`

### Problem: "Failed to read from DHT sensor"
**Solutions:**
- Check DHT11 wiring (VCC to 5V, GND to GND, DATA to Pin 2)
- DHT11 may need 1-2 seconds to stabilize after power on
- Try a different DHT11 sensor (might be faulty)
- Add 10kΩ pull-up resistor between DATA and VCC (optional)

### Problem: LED doesn't turn on/off
**Solutions:**
- Check LED polarity (longer leg should go to resistor)
- Verify resistor is 220Ω (red-red-brown bands)
- Check Pin 13 connection
- Test LED with a battery to verify it works
- Check if built-in LED on Pin 13 responds (Arduino Uno has one)

### Problem: No telemetry data appearing
**Solutions:**
- Wait 2 seconds for first reading
- Check serial connection (look for "Connected to Arduino" message)
- Verify baud rate is 9600 in both Arduino code and Python
- Restart Arduino (unplug and replug USB)

### Problem: "ModuleNotFoundError: No module named 'serial'"
**Solutions:**
- Install pyserial: `pip install pyserial`
- Verify Python installation: `python --version`
- Try: `pip3 install pyserial` if using Python 3

### Problem: Data not saving to CSV
**Solutions:**
- Check write permissions in project folder
- Look for `sensor_data.csv` in project root directory
- Run Python as administrator (Windows) if permission denied

---

## Quick Reference

### Arduino Pins Used
| Component | Arduino Pin |
|-----------|-------------|
| DHT11 DATA | Digital Pin 2 |
| LED | Digital Pin 13 |
| DHT11 VCC | 5V |
| DHT11 GND, LED GND | GND |

### Serial Commands (for manual testing)
You can test Arduino directly using Arduino IDE Serial Monitor:
- `LED:ON` - Turn LED on
- `LED:OFF` - Turn LED off
- `STATUS` - Get system status

Set Serial Monitor to 9600 baud and "Newline" ending.

### File Structure
```
smarthome/
├── Arduino/
│   └── smarthome_checkin2.ino    (Arduino code)
├── UI/
│   └── smarthome_ui.py           (Python console UI)
├── sensor_data.csv                (Generated data file)
└── SETUP_INSTRUCTIONS.md          (This file)
```

---

## For Check-In 2 Demonstration

### What to Show Professor:
1. **Working hardware:** DHT11 connected, LED connected
2. **Telemetry streaming:** Temperature data updating every 2 seconds (MQTT)
3. **LED control:** Turn LED on and off via UI (REST API)
4. **Data storage:** Show `sensor_data.csv` with collected data
5. **Code organization:** Show Arduino and UI folders

### What to Explain:
- **MQTT usage:** Continuous temperature telemetry simulated via serial
- **REST API usage:** Command-response pattern for LED control
- **Two hardware capabilities:** DHT11 sensor (input) + LED (output)
- **Data persistence:** CSV file stores all readings with timestamps

---

## Next Steps for Final Submission

For the final capstone, consider:
1. Implementing actual MQTT broker (e.g., Mosquitto)
2. Implementing actual REST API server (e.g., Flask)
3. Adding WiFi capability (ESP32 or Arduino with WiFi shield)
4. Improving UI (web interface, charts, alerts)
5. Adding more sensors or outputs
6. Implementing threshold-based alerts

---

**Team SmartSense**
*Making small spaces smarter, one room at a time*
