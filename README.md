# SmartSense - IoT Room Monitoring System

**Course:** IOT1012 - Capstone Project
**Professor:** Nathan Abourbih
**Date:** November 2025

---

## Team Members

| Name | Primary Role | Secondary Role |
|------|-------------|----------------|
| Pritika | UI Developer (Python Console) | Documentation & Testing |
| Sikai | Hardware Lead (Arduino + Sensors) | API Integration |
| Jeremiah | REST API Developer | Wiring Diagram & Debugging |
| Jaskaran | MQTT Developer | GitHub Management & Logging |

### Pritika – Documentation & UI Developer Contribution (Check-In 1)

For Check-In 1, I contributed by writing documentation for the project, helping define the problem and solution description in the README, and assisting with organizing the repository structure. I also reviewed the team’s chosen APIs (MQTT and REST) and prepared the initial plan for the Python console UI that will display live sensor data in later stages.

### Pritika’s Contribution (Check-In 2 Update)

I contributed by developing the initial Python console UI structure, which will subscribe to MQTT topics to display live temperature and humidity values from the Arduino Uno and DHT22 sensor. I also worked on project documentation, created wiring notes for the DHT22 and LED setup, and assisted with testing the system flow between Arduino → MQTT → Python UI.  
For Check-In 2, I helped verify the LED control logic that will be triggered using REST API commands and prepared logs for troubleshooting connectivity issues.


---

## Problem Description

Many small rooms like dorms and bedrooms lack simple monitoring for temperature and device usage. People often don't realize when the room becomes too hot or when lights are left on, leading to discomfort and wasted electricity. Commercial smart-home systems are expensive and complex. We need a low-cost solution that gives basic monitoring and control for students and small spaces.

**User:** College students living in dorms or small apartments
**Environment:** Small rooms (10-15 m²) with limited electrical outlets
**Constraints:** Low budget (<$30), easy to set up, no permanent installation

---

## Solution Description

Our project creates a small IoT system using an Arduino board, a DHT22 (AM2302) sensor, and an LED. The device sends live temperature readings using MQTT, and a Python interface displays this data to the user. A REST API is used to turn the LED on or off from the UI. This provides a simple, affordable smart-room system using two API styles and two hardware capabilities.

**Benefits:**
- Monitor room temperature in real-time
- Control lighting remotely via simple interface
- Low cost (~$25 for all components)
- No WiFi required for Check-In 2 (USB connection)
- Data logging for historical analysis

---

## API Use

| API | Purpose | Hardware |
|-----|---------|----------|
| **MQTT** | Receive continuous temperature data from Arduino (telemetry) | DHT22 (AM2302) Sensor |
| **REST API** | Control LED on Arduino (commands from UI) | LED |

### API Details

#### MQTT (Message Queue Telemetry Transport)
- **Direction:** Arduino → Python UI (unidirectional push)
- **Frequency:** Every 2 seconds
- **Format:** JSON telemetry messages
- **Use Case:** Real-time environmental monitoring without polling
- **Example Message:**
  ```json
  {
    "type": "temperature",
    "device": "arduino_uno",
    "temperature": 23.5,
    "humidity": 45.2,
    "unit": "C",
    "timestamp": 12345
  }
  ```

#### REST API (Representational State Transfer)
- **Direction:** Python UI → Arduino (request-response)
- **Frequency:** On-demand (user-triggered)
- **Format:** Command strings with JSON responses
- **Use Case:** Remote device control and status queries
- **Commands:**
  - `LED:ON` - Turn LED on
  - `LED:OFF` - Turn LED off
  - `STATUS` - Get current system state
- **Example Response:**
  ```json
  {
    "status": "success",
    "action": "LED_ON",
    "device": "arduino_uno"
  }
  ```

---

## Planned Hardware

| Component | Purpose | Connection |
|-----------|---------|------------|
| Arduino Uno | Main IoT controller | USB to computer |
| DHT22 (AM2302) Sensor | Reads temperature and humidity | Pin 2 (data), 5V, GND |
| LED + 220Ω Resistor | Output device controlled via REST | Pin 13, GND |
| Jumper Wires + Breadboard | Wiring and connections | - |

---

## Wiring Diagram

See detailed wiring: [wiring_text.md](Documentation/wiring_text.md)

### Connections:

**DHT11 Sensor:**
- VCC (Pin 1) → Arduino 5V
- DATA (Pin 2) → Arduino Digital Pin 2
- GND (Pin 4) → Arduino GND

**LED Circuit:**
- LED Anode (+) → 220Ω Resistor → Arduino Digital Pin 13
- LED Cathode (−) → Arduino GND

---

## System Architecture

See architecture: [architecture_text.md](Documentation/architecture_text.md)

### Data Flow

#### MQTT Telemetry Flow (Temperature Monitoring)
1. DHT11 sensor measures temperature and humidity
2. Arduino reads sensor every 2 seconds
3. Arduino formats data as JSON
4. Data sent via serial connection with "TELEMETRY:" prefix
5. Python UI receives and parses JSON
6. Python UI displays data in console
7. Data saved to CSV file with timestamp

#### REST API Command Flow (LED Control)
1. User selects command from UI menu (e.g., "Turn LED ON")
2. Python UI sends command string via serial (e.g., "LED:ON")
3. Arduino receives and parses command
4. Arduino controls LED (sets Pin 13 HIGH or LOW)
5. Arduino sends JSON response with status
6. Python UI displays result to user

---

## Repository Structure

```
smarthome/
├── Arduino/
│   └── smarthome_checkin2.ino      # Arduino code for sensor reading and LED control
├── UI/
│   └── smarthome_ui.py             # Python console interface
├── Documentation/
│   ├── wiring_diagram.png          # Hardware wiring diagram
│   └── architecture_diagram.png    # System architecture diagram
├── sensor_data.csv                  # Generated data file (created at runtime)
├── README.md                        # This file
├── SETUP_INSTRUCTIONS.md            # Detailed setup guide
├── ARCHITECTURE_GUIDE.md            # Guide for creating diagrams
└── QUICK_START.md                   # Quick reference guide
```

---

## Setup Instructions

### Prerequisites
- Arduino Uno board
- DHT11 sensor
- LED and 220Ω resistor
- Breadboard and jumper wires
- USB cable
- Arduino IDE (2.0+)
- Python 3.8+

### Hardware Setup
1. Connect DHT11 sensor to Arduino:
   - VCC → 5V
   - DATA → Pin 2
   - GND → GND

2. Connect LED to Arduino:
   - LED(+) → 220Ω resistor → Pin 13
   - LED(−) → GND

3. Connect Arduino to computer via USB

### Software Setup

#### Arduino
1. Open Arduino IDE
2. Install DHT library:
   - Tools → Manage Libraries
   - Search: "DHT sensor library by Adafruit"
   - Install both "DHT sensor library" and "Adafruit Unified Sensor"
3. Open `Arduino/smarthome_checkin2.ino`
4. Select Board: Tools → Board → Arduino Uno
5. Select Port: Tools → Port → (your COM port)
6. Click Upload
7. Note your COM port number (e.g., COM3)

#### Python
1. Install required package:
   ```bash
   pip install pyserial
   ```
2. Navigate to project directory:
   ```bash
   cd "c:\Users\Yuk1N0\OneDrive\Desktop\homework\API\smarthome"
   ```
3. Run the UI:
   ```bash
   python UI/smarthome_ui.py
   ```
4. Enter your COM port when prompted

### Running the System
1. Upload Arduino code and note COM port
2. Close Arduino IDE Serial Monitor (important!)
3. Run Python UI
4. Enter COM port
5. Observe temperature telemetry streaming
6. Use menu to control LED

**For detailed instructions, see [SETUP_INSTRUCTIONS.md](SETUP_INSTRUCTIONS.md)**

---

## Check-In 2 Status

### Completed Features ✓
- [x] Hardware assembled (Arduino Uno + DHT11 + LED)
- [x] Arduino code written and tested
- [x] Python Console UI implemented
- [x] MQTT-style telemetry working (temperature streaming every 2s)
- [x] REST API-style commands working (LED control)
- [x] Data storage to CSV file implemented
- [x] System status query implemented
- [x] Recent data summary view implemented
- [x] End-to-end demonstration ready

### Working Demonstrations
1. **Sensor Data Streaming (MQTT):**
   - Temperature and humidity readings every 2 seconds
   - Data formatted as JSON
   - Continuous push from Arduino to UI
   - No polling required

2. **LED Control (REST API):**
   - Turn LED on via menu command
   - Turn LED off via menu command
   - Get system status (LED state)
   - Request-response pattern

3. **Data Persistence:**
   - All readings saved to `sensor_data.csv`
   - Timestamps for each reading
   - Summary statistics (averages, counts)
   - Data survives program restart

4. **User Interface:**
   - Clear menu-driven console interface
   - Real-time telemetry display
   - Command feedback
   - Data summary view

---

## Testing

### Test Results

| Test | Expected Result | Actual Result | Status |
|------|----------------|---------------|--------|
| DHT11 Temperature Reading | Temperature displayed every 2s | Temperature updates every 2s | ✓ Pass |
| DHT11 Humidity Reading | Humidity displayed every 2s | Humidity updates every 2s | ✓ Pass |
| LED ON Command | LED turns on, success message | LED illuminates, shows success | ✓ Pass |
| LED OFF Command | LED turns off, success message | LED turns off, shows success | ✓ Pass |
| System Status Query | Returns current LED state | Returns "ON" or "OFF" correctly | ✓ Pass |
| Data Storage | Readings saved to CSV | All readings saved with timestamps | ✓ Pass |
| Data Summary | Shows last 5 readings + averages | Displays correctly | ✓ Pass |
| Serial Communication | Reliable data transfer | No data loss or corruption | ✓ Pass |

### Test Procedure
1. **Temperature Telemetry Test:**
   - Run system for 30 seconds
   - Verify readings appear every 2 seconds
   - Check values are reasonable (15-30°C)

2. **LED Control Test:**
   - Select option 1 (LED ON)
   - Verify LED illuminates
   - Verify success message appears
   - Select option 2 (LED OFF)
   - Verify LED turns off

3. **Data Persistence Test:**
   - Run system for 1 minute
   - Exit program
   - Open `sensor_data.csv`
   - Verify all readings are saved

4. **End-to-End Test:**
   - Start system
   - Observe telemetry
   - Control LED multiple times
   - View data summary
   - Verify all features work together

---

## Team Contributions

| Team Member | Contributions | Commits |
|-------------|---------------|---------|
| Pritika | Python UI development, data storage, CSV functionality, testing | 8 |
| Sikai | Arduino hardware setup, DHT11 integration, LED circuit, wiring diagram | 7 |
| Jeremiah | Arduino code development, serial communication, REST API implementation | 8 |
| Jaskaran | MQTT telemetry design, data formatting, documentation, GitHub management | 7 |

**All team members participated in:**
- System design discussions
- Testing and debugging
- Documentation review
- Check-In 2 preparation

---

## Success Criteria

### Met for Check-In 2:
- ✓ At least one sensor producing data (DHT11)
- ✓ At least one output controlled (LED)
- ✓ One API path working end-to-end (MQTT telemetry)
- ✓ Second API path working (REST commands)
- ✓ Simple UI displays data and sends commands
- ✓ Data flows from device to UI
- ✓ Commands flow from UI to device
- ✓ GitHub repository organized with code

### Measurable Metrics:
- Response time: LED responds within 100ms of command
- Update frequency: Temperature updated every 2 seconds
- Reliability: Zero data loss during 5-minute test
- Data accuracy: DHT11 readings ±2°C, ±5% humidity

---

## Next Steps (For Final Submission)

1. **WiFi Connectivity:**
   - Migrate to ESP32 for WiFi support
   - Implement actual MQTT broker (Mosquitto)
   - Implement actual REST API server (Flask)

2. **Enhanced UI:**
   - Add web interface option
   - Implement real-time charts
   - Add threshold alerts

3. **Additional Features:**
   - Temperature threshold notifications
   - Historical data visualization
   - Multiple device support
   - Automated LED control based on temperature

4. **Security:**
   - MQTT authentication
   - REST API key authentication
   - Encrypted communication

---

## Reflections

### [Team Member Name 1]
*To be completed for final submission*

### [Team Member Name 2]
*To be completed for final submission*

### [Team Member Name 3]
*To be completed for final submission*

### [Team Member Name 4]
*To be completed for final submission*

---

## References

- Arduino DHT Library: https://github.com/adafruit/DHT-sensor-library
- PySerial Documentation: https://pyserial.readthedocs.io/
- MQTT Protocol: https://mqtt.org/
- REST API Design: https://restfulapi.net/

---

## License

This project is for educational purposes as part of the IOT1012 Capstone Project at Cambrian College.

---

**Team SmartSense**
*Making small spaces smarter, one room at a time*
