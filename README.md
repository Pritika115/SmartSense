# SmartSense IoT System - Setup Guide

## Team Members
- **Pritika**: UI Developer (Python Console)
- **Sikai**: Hardware Lead (ESP32 + Sensors)
- **Jeremiah**: REST API Developer
- **Jaskaran**: MQTT Developer

## Project Overview

SmartSense is an IoT room monitoring system that:
- **MQTT**: Continuously publishes temperature & humidity data (telemetry)
- **REST API**: Allows control of LED state (on/off)
- Uses ESP32-WROOM-32 with DHT22 sensor and LED

---

## Hardware Requirements

1. **ESP32-WROOM-32** development board
2. **DHT22** (AM2302) temperature & humidity sensor
3. **LED** (any color)
4. **220Ω resistor**
5. **Breadboard and jumper wires**

---

## Wiring Diagram

```
DHT22 Sensor:
  - VCC  → ESP32 3.3V
  - GND  → ESP32 GND
  - DATA → ESP32 GPIO 4

LED:
  - Anode (+, longer leg) → 220Ω resistor → ESP32 GPIO 18
  - Cathode (-, shorter leg) → ESP32 GND
```

**Important Notes:**
- DHT22 can work with 3.3V or 5V (3.3V is sufficient)
- The 220Ω resistor prevents LED from burning out
- GPIO 4 and GPIO 18 are configurable in code

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

**Good luck with your presentation! 🚀**
