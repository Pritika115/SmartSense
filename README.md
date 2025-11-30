# SmartSense IoT System

## Team Members
- **Pritika**: UI Developer (Python Console)
- **Sikai**: Hardware Lead (ESP32 + Sensors)
- **Jeremiah**: REST API Developer
- **Jaskaran**: MQTT Developer

---

## Problem Description

Students living in dormitories or shared accommodations often face challenges in maintaining optimal room conditions for studying and sleeping. Temperature and humidity levels can significantly impact comfort, health, and academic performance. However, most students lack affordable, real-time monitoring solutions that can alert them to poor environmental conditions.

**Key Problems:**
- No easy way to monitor room temperature and humidity remotely
- Difficulty knowing when to adjust heating/cooling or ventilation
- Lack of historical data to identify patterns in room conditions
- Need for simple, low-cost IoT solution accessible to students

---

## Solution Description

SmartSense is an affordable IoT room monitoring system that provides real-time environmental data and remote control capabilities. The system uses an ESP32 microcontroller with a DHT22 sensor to continuously monitor temperature and humidity, transmitting data via MQTT protocol for real-time telemetry.

**Key Features:**
- **Real-time Monitoring**: Continuous temperature and humidity readings every 5 seconds
- **Remote Control**: LED indicator that can be controlled via REST API (demonstrates actuator control)
- **Dual API Architecture**:
  - MQTT for efficient, continuous telemetry streaming
  - REST API for on-demand status queries and device control
- **User-Friendly Interface**: Python console application for easy monitoring and control
- **Low Cost**: Total hardware cost under $15 (ESP32 + DHT22 + LED)

---

## Who Benefits

1. **Students**: Monitor room conditions to optimize study environment and ensure comfortable sleeping conditions
2. **Dormitory Managers**: Can deploy multiple units to monitor building-wide environmental conditions
3. **Health-Conscious Individuals**: Track humidity levels to prevent mold growth and respiratory issues
4. **Budget-Conscious Users**: Affordable alternative to commercial smart home sensors ($100+)
5. **IoT Learners**: Excellent educational platform for understanding MQTT, REST APIs, and embedded systems

---

## System Architecture

The SmartSense system follows a distributed IoT architecture with clear separation between telemetry (MQTT) and command/control (REST API).

```
┌─────────────┐
│    USER     │
│  (Student)  │
└──────┬──────┘
       │ Interacts via console
       ↓
┌─────────────────────────────────┐
│   PYTHON CLIENT UI              │
│   (smartsense_client.py)        │
│                                 │
│  - Subscribe to MQTT telemetry  │
│  - Send REST API commands       │
│  - Display real-time data       │
└────────┬────────────────┬───────┘
         │                │
         │ MQTT           │ REST/HTTP
         │ (Port 1883)    │ (Port 80)
         ↓                ↓
    ┌─────────┐    ┌──────────┐
    │  MQTT   │    │          │
    │ Broker  │    │          │
    │ HiveMQ  │    │          │
    └────┬────┘    │          │
         ↑         │          │
         │         │          │
         │ Publish │  HTTP    │
         │         │  Request │
         └─────────┴──────────┘
                   │
         ┌─────────┴─────────┐
         │                   │
┌────────┴────────────────────┴──────┐
│   ESP32-WROOM-32 MICROCONTROLLER   │
│                                    │
│  WiFi Module                       │
│  MQTT Publisher                    │
│  Web Server (REST API)             │
│  Sensor Interface                  │
└─────┬────────────┬─────────────────┘
      │            │
      ↓            ↓
┌──────────┐  ┌─────────┐
│  DHT22   │  │   LED   │
│  Sensor  │  │ GPIO 18 │
│          │  │         │
│ Temp &   │  │ Status  │
│ Humidity │  │ Light   │
└──────────┘  └─────────┘
```

**Architecture Diagram:** See [Documentation/architecture_text.md](Documentation/architecture_text.md)

---

## Component Roles

### Hardware Components

| Component | Role | Purpose |
|-----------|------|---------|
| **ESP32-WROOM-32** | Main microcontroller | Manages WiFi connectivity, runs MQTT client, hosts web server, interfaces with sensors |
| **DHT22 Sensor** | Temperature & Humidity sensor | Measures ambient temperature (°C) and relative humidity (%) with ±0.5°C and ±2% accuracy |
| **LED** | Visual indicator & actuator demo | Demonstrates remote control capability; can indicate system status or alerts |
| **220Ω Resistor** | Current limiter | Protects LED from overcurrent damage |

### Software Components & APIs

| Component | API Type | Role | Why This API? |
|-----------|----------|------|---------------|
| **MQTT Telemetry** | MQTT (Pub/Sub) | Continuously publishes temperature and humidity data to broker | **Ideal for streaming sensor data**: Low bandwidth, efficient for continuous updates, supports many subscribers, works over unreliable networks |
| **REST API** | HTTP/REST | Provides endpoints for LED control and system status queries | **Ideal for command/control**: Request-response pattern, familiar HTTP protocol, easy to test with curl/browsers, stateless operations |
| **Python Client** | Consumer | Subscribes to MQTT topics and sends REST API requests | Provides unified interface for both APIs; demonstrates how to integrate both paradigms |
| **Data Storage** | Local state | ESP32 maintains current sensor readings and LED state in memory | Enables `/status` endpoint to return complete system state on demand |

### Why Two Different APIs?

- **MQTT**: Best for **telemetry** - data flows continuously from device to cloud/clients without polling
- **REST API**: Best for **control** - user initiates specific actions (turn LED on/off) and expects immediate response

This demonstrates **API pattern matching** - choosing the right protocol for the right job.

---

## Hardware Requirements

1. **ESP32-WROOM-32** development board
2. **DHT22** (AM2302) temperature & humidity sensor
3. **LED** (any color)
4. **220Ω resistor**
5. **Breadboard and jumper wires**

---

## Wiring Diagram

**Visual Diagram:** See [Documentation/wiring_diagram.txt](Documentation/wiring_diagram.txt)

### Pin Connections

**DHT22 Sensor:**
- VCC (Power) → ESP32 3.3V
- DATA (Signal) → ESP32 GPIO 4
- GND (Ground) → ESP32 GND

**LED Circuit:**
- Anode (+, longer leg) → 220Ω resistor → ESP32 GPIO 18
- Cathode (-, shorter leg) → ESP32 GND

**Important Notes:**
- DHT22 can work with 3.3V or 5V (3.3V is sufficient)
- The 220Ω resistor prevents LED from burning out
- GPIO 4 and GPIO 18 are configurable in code
- All grounds must be connected together (common ground)

---

## Software Setup

### Step 1: Install Arduino IDE

1. Download from: https://www.arduino.cc/en/software
2. Install the IDE

### Step 2: Install ESP32 Board Support

1. Open Arduino IDE
2. Go to **File → Preferences**
3. Add this URL to "Additional Board Manager URLs":
   ```
   https://dl.espressif.com/dl/package_esp32_index.json
   ```
4. Go to **Tools → Board → Boards Manager**
5. Search for "esp32" and install "ESP32 by Espressif Systems"

### Step 3: Install Required Libraries

Go to **Sketch → Include Library → Manage Libraries** and install:

1. **DHT sensor library** by Adafruit (version 1.4.4 or later)
2. **Adafruit Unified Sensor** (dependency for DHT library)
3. **PubSubClient** by Nick O'Leary (for MQTT)
4. **ArduinoJson** by Benoit Blanchon (version 6.x)

### Step 4: Configure the ESP32 Code

1. Open `smartsense_esp32.ino` in Arduino IDE
2. Update these lines with your WiFi credentials:
   ```cpp
   const char* ssid = "YOUR_WIFI_SSID";
   const char* password = "YOUR_WIFI_PASSWORD";
   ```
3. Select your board: **Tools → Board → ESP32 Arduino → ESP32 Dev Module**
4. Select your port: **Tools → Port → (select your COM port)**
5. Upload the code

### Step 5: Get ESP32 IP Address

1. Open Serial Monitor: **Tools → Serial Monitor**
2. Set baud rate to **115200**
3. Press the ESP32 reset button
4. Note the IP address displayed (e.g., `192.168.1.105`)

---

## Python Client Setup

### Step 1: Install Python

Ensure Python 3.7+ is installed:
```bash
python --version
```

### Step 2: Install Dependencies

```bash
pip install -r requirements.txt
```

Or manually:
```bash
pip install paho-mqtt==1.6.1 requests==2.31.0
```

### Step 3: Configure Python Client

Edit `smartsense_client.py` and update:
```python
ESP32_IP = "192.168.1.XXX"  # Replace with your ESP32 IP
```

### Step 4: Run the Client

```bash
python smartsense_client.py
```

---

## Testing the System

### Test 1: MQTT Telemetry (Temperature & Humidity)

1. Power on ESP32
2. Run Python client: `python smartsense_client.py`
3. You should see temperature and humidity readings every 5 seconds
4. Breathe on the DHT22 sensor to see temperature/humidity increase

**Expected Output:**
```
[2024-11-28 10:30:15]
  🌡️  Temperature: 22.3°C
  💧 Humidity:    45.2%
--------------------------------------------------
```

### Test 2: REST API (LED Control)

From the Python client menu:
1. Press `1` to turn LED ON → LED should light up
2. Press `2` to turn LED OFF → LED should turn off
3. Press `3` to get system status

**Alternative Testing with curl:**

Turn LED ON:
```bash
curl -X POST http://192.168.1.XXX/led \
  -H "Content-Type: application/json" \
  -d '{"state":"on"}'
```

Turn LED OFF:
```bash
curl -X POST http://192.168.1.XXX/led \
  -H "Content-Type: application/json" \
  -d '{"state":"off"}'
```

Get Status:
```bash
curl http://192.168.1.XXX/status
```

---

## API Documentation

### MQTT Topics

**Published by ESP32:**
- `smartsense/temperature` - Temperature in Celsius (published every 5 seconds)
- `smartsense/humidity` - Humidity percentage (published every 5 seconds)

**MQTT Broker:** `broker.hivemq.com:1883` (free public broker)

### REST API Endpoints

**Base URL:** `http://<ESP32_IP>`

#### 1. GET `/`
Get API documentation page

**Response:** HTML page with endpoint list

---

#### 2. GET `/status`
Get complete system status

**Response:**
```json
{
  "temperature": 23.5,
  "humidity": 48.2,
  "led_state": "on",
  "wifi_rssi": -55,
  "ip_address": "192.168.1.105"
}
```

---

#### 3. GET `/led`
Get current LED state

**Response:**
```json
{
  "led_state": "on"
}
```

---

#### 4. POST `/led`
Control LED state

**Request Body:**
```json
{
  "state": "on"
}
```
or
```json
{
  "state": "off"
}
```

**Response (Success):**
```json
{
  "led_state": "on",
  "message": "LED turned on"
}
```

**Response (Error):**
```json
{
  "error": "Invalid state. Use 'on' or 'off'"
}
```

---

## Test Results

The following tests were performed to validate system functionality:

| Test # | Test Description | Expected Result | Actual Result | Status |
|--------|-----------------|-----------------|---------------|--------|
| 1 | ESP32 WiFi Connection | ESP32 connects to WiFi and displays IP address in Serial Monitor | ESP32 connected successfully, IP: 192.168.1.105 | ✅ PASS |
| 2 | DHT22 Sensor Reading | Temperature and humidity readings appear in Serial Monitor every 5 seconds | Readings displayed correctly: ~22°C, ~45% humidity | ✅ PASS |
| 3 | MQTT Publishing | Temperature and humidity data published to MQTT broker topics | Data successfully published to `smartsense/temperature` and `smartsense/humidity` | ✅ PASS |
| 4 | MQTT Subscription | Python client receives and displays MQTT telemetry data | Client receives updates every 5 seconds, displays formatted output | ✅ PASS |
| 5 | REST API - GET /status | Returns JSON with complete system status | Returns valid JSON with temp, humidity, LED state, WiFi RSSI, IP address | ✅ PASS |
| 6 | REST API - GET /led | Returns current LED state | Returns `{"led_state":"off"}` correctly | ✅ PASS |
| 7 | REST API - POST /led (ON) | LED turns on and returns success message | LED illuminated, received `{"led_state":"on","message":"LED turned on"}` | ✅ PASS |
| 8 | REST API - POST /led (OFF) | LED turns off and returns success message | LED turned off, received `{"led_state":"off","message":"LED turned off"}` | ✅ PASS |
| 9 | REST API - Invalid State | Returns error message for invalid state | Received `{"error":"Invalid state. Use 'on' or 'off'"}` | ✅ PASS |
| 10 | Python UI Menu | User can select options to control LED and view status | All menu options (1-4) work correctly | ✅ PASS |
| 11 | Sensor Response Test | Breathing on DHT22 increases temp/humidity readings | Temperature increased from 22°C to 24°C, humidity from 45% to 60% | ✅ PASS |
| 12 | MQTT Reconnection | System reconnects to MQTT broker after disconnect | Successfully reconnected after network interruption | ✅ PASS |
| 13 | Concurrent Operation | MQTT telemetry continues while REST API is being used | Both protocols operate simultaneously without interference | ✅ PASS |
| 14 | Web Browser Access | Opening `http://<ESP32_IP>` in browser shows API documentation | HTML page displays endpoint list correctly | ✅ PASS |

**Test Summary:**
- Total Tests: 14
- Passed: 14
- Failed: 0
- Success Rate: 100%

---

## Troubleshooting

### ESP32 won't connect to WiFi
- Check SSID and password are correct
- Ensure WiFi is 2.4GHz (ESP32 doesn't support 5GHz)
- Check if network allows IoT devices

### DHT22 returns NaN
- Check wiring connections
- Ensure DHT22 has power (3.3V or 5V)
- Wait 2 seconds after power-on for sensor to initialize
- Try adding a 10kΩ pull-up resistor between DATA and VCC

### MQTT not receiving data
- Verify internet connection
- Try alternative MQTT broker: `test.mosquitto.org`
- Check firewall isn't blocking port 1883

### REST API not responding
- Verify ESP32 IP address with Serial Monitor
- Ensure Python client has correct IP
- Check ESP32 and computer are on same network
- Try accessing `http://<ESP32_IP>` in web browser

### LED not working
- Check LED polarity (longer leg = anode/+)
- Verify 220Ω resistor is in series
- Test LED with multimeter or different GPIO pin

---

## Project Structure

```
smartsense/
├── smartsense_esp32.ino    # ESP32 firmware (Arduino)
├── smartsense_client.py    # Python MQTT subscriber & REST client
├── requirements.txt        # Python dependencies
└── README.md              # This file
```

---

## Key Features Demonstrated

✅ **MQTT Protocol** - Publish/Subscribe for telemetry data
✅ **REST API** - HTTP endpoints for device control
✅ **DHT22 Sensor** - Temperature and humidity monitoring
✅ **LED Control** - Remote device actuation
✅ **JSON** - Structured data format
✅ **IoT Communication** - Real-world sensor-to-cloud pattern

---

## Future Enhancements

- Add MQTT authentication (username/password)
- Implement data logging to SD card or database
- Add more sensors (light, motion, air quality)
- Create web dashboard instead of console UI
- Add MQTT commands to control LED (not just REST)
- Implement OTA (Over-The-Air) firmware updates

---

## Presentation Tips

1. **Live Demo**: Show temperature changing in real-time
2. **API Comparison**: Explain when to use MQTT vs REST
   - MQTT: Continuous sensor data (telemetry)
   - REST: On-demand control (commands)
3. **Show Serial Monitor**: Display ESP32 logs during demo
4. **Network Architecture**: Draw diagram showing data flow
5. **Cost Analysis**: Emphasize low-cost solution (~$10-15)

---

## Additional Resources

- ESP32 Documentation: https://docs.espressif.com/
- MQTT Protocol: https://mqtt.org/
- DHT22 Datasheet: https://www.sparkfun.com/datasheets/Sensors/Temperature/DHT22.pdf
- REST API Design: https://restfulapi.net/

---

## License

Educational project for Cambrian College IoT course.

---

## Student Reflections

### Pritika - UI Developer
> "Working on the SmartSense project taught me how different API protocols serve different purposes in IoT systems. I learned that MQTT is ideal for continuous data streaming from sensors because of its lightweight pub/sub model, while REST APIs work better for command-and-control operations. Implementing the Python client helped me understand asynchronous programming and how to handle real-time data streams. The most challenging part was coordinating between two different communication protocols in a single application, but it taught me valuable lessons about API integration. This project showed me that choosing the right API for the right task is just as important as writing clean code."

### Sikai - Hardware Lead
> "This project deepened my understanding of embedded systems and IoT protocols. Setting up the ESP32 with the DHT22 sensor taught me about sensor interfacing, timing considerations, and the importance of proper wiring and pull-up resistors. I learned how WiFi-enabled microcontrollers can serve dual roles as both MQTT publishers and HTTP servers simultaneously. Troubleshooting connection issues helped me develop better debugging skills using Serial Monitor. The experience of integrating hardware with two different software protocols showed me the practical applications of IoT in solving real-world problems. I now understand why ESP32 is so popular in IoT projects - its WiFi capabilities and processing power make it incredibly versatile."

### Jeremiah - REST API Developer
> "Developing the REST API on the ESP32 taught me about resource-constrained web servers and the importance of lightweight JSON responses. I learned how to structure RESTful endpoints following HTTP conventions even on embedded devices with limited memory. Implementing proper error handling and JSON serialization/deserialization was challenging but rewarding. The project showed me that REST APIs aren't just for cloud services - they work great for IoT device control too. Understanding when to use stateless HTTP requests versus stateful MQTT connections gave me a better appreciation for API design patterns. This hands-on experience with ESP32 WebServer library will be valuable for future IoT projects."

### Jaskaran - MQTT Developer
> "Implementing MQTT functionality taught me about the publish/subscribe messaging pattern and how it differs from traditional request/response models. I learned to work with the PubSubClient library and understood the importance of Quality of Service levels in IoT communication. Connecting to a public MQTT broker (HiveMQ) showed me how easy it is to build distributed systems without managing server infrastructure. The most interesting challenge was ensuring reliable MQTT reconnection when network interruptions occur. This project helped me understand why MQTT is the standard for IoT telemetry - its efficiency and reliability make it perfect for sensor networks. I now have practical experience that complements my theoretical knowledge of IoT protocols."

---

## Team Contributions

| Team Member | Role | Contributions | Commits |
|-------------|------|---------------|---------|
| **Pritika** | UI Developer | Developed Python console UI (`smartsense_client.py`), implemented MQTT subscriber, created REST API client functions, designed user menu interface, wrote UI documentation | 4+ |
| **Sikai** | Hardware Lead | Wired ESP32 with DHT22 and LED, configured GPIO pins, troubleshot hardware issues, created wiring diagrams, tested sensor accuracy, documented pin connections | 4+ |
| **Jeremiah** | REST API Developer | Implemented REST endpoints (`/status`, `/led`, `/`), designed JSON response format, added error handling, tested API with curl and browser, documented API specifications | 4+ |
| **Jaskaran** | MQTT Developer | Implemented MQTT publishing logic, configured HiveMQ broker connection, added reconnection handling, tested telemetry streaming, documented MQTT topics and QoS | 4+ |

**Shared Responsibilities:**
- All members contributed to README.md documentation
- All members participated in system testing and validation
- All members collaborated on architecture design decisions
- All members reviewed and debugged each other's code

---

## Project Structure

```
SmartSense/
├── Arduino/
│   └── smartsense_esp32/
│       └── smartsense_esp32.ino      # ESP32 firmware (MQTT + REST API)
├── UI/
│   ├── smartsense_client.py          # Python console UI (MQTT subscriber + REST client)
│   ├── requirements.txt              # Python dependencies
│   └── smarthome_ui.py               # Legacy Check-In 2 UI
├── Documentation/
│   ├── architecture_text.md          # System architecture diagram
│   ├── wiring_diagram.txt            # Hardware wiring diagram
│   └── wiring_text.md                # Wiring description
├── Screenshots/                       # Testing screenshots
├── Reflections/                       # Individual reflection documents
├── README.md                          # This file (complete documentation)
├── SETUP_INSTRUCTIONS.md              # Quick setup guide
├── QUICK_START.md                     # Quick start guide
└── CHECKIN2_CHECKLIST.md             # Check-In 2 requirements

```

---

## Key Features Demonstrated

✅ **Dual API Architecture** - MQTT for telemetry, REST for control
✅ **Real-time Data Streaming** - Continuous sensor publishing
✅ **Remote Device Control** - HTTP-based LED actuation
✅ **WiFi Connectivity** - ESP32 wireless communication
✅ **Sensor Integration** - DHT22 temperature/humidity monitoring
✅ **JSON Data Format** - Structured data exchange
✅ **Error Handling** - Graceful failure management
✅ **User Interface** - Interactive Python console
✅ **Data Storage** - Local state management on ESP32
✅ **Simple Reporting** - Real-time display and system status queries

---

## License

Educational project for Cambrian College IoT Systems course.

---

**Project Complete - Ready for Final Submission**
