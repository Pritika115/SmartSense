# SmartSense – Check-In 2 Report  
**Student:** Jeremiah  
**Course:** IoT1012  
**Date:** Nov 2025  
**Hardware:** Arduino Uno + DHT22 + LED  
**Software:** Python Flask REST API + Serial Integration  

---

## 1. Overview

This report documents the successful integration between the **Arduino Uno hardware** and the **Python REST API** for the SmartSense system.  
The goal of Check-In 2 is to demonstrate:

- Serial communication between Python and Arduino  
- Valid JSON responses from the microcontroller  
- Working REST API endpoints  
- Real sensor values (temperature + humidity)  
- Remote LED control via `/api/led/on` and `/api/led/off`

All system components function correctly.

---

## 2. System Architecture (Summary)

**Python (Flask) REST API**  
- Sends commands to Arduino over Serial  
- Parses Arduino JSON responses  
- Exposes endpoints for UI (health, status, LED on/off)

**Arduino Uno**  
- Reads DHT22 temp/humidity  
- Controls LED on command  
- Returns JSON output ONLY (no noise/extra text)

Communication uses **9600 baud** over COM8.

---

## 3. Arduino Code Used
/* 
 * SmartSense - Check-In 2
 * TEAM MEMBER: Jeremiah
 * Arduino Uno + DHT22 + LED
 *
 * Clean JSON-based communication with Python.
 */

#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

#define LED_PIN 7

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(LED_PIN, OUTPUT);

  Serial.println("{\"system\":\"started\"}");
}

void loop() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command == "LED:ON") {
      digitalWrite(LED_PIN, HIGH);
      Serial.println("{\"status\":\"success\",\"led_state\":\"ON\"}");
    }
    else if (command == "LED:OFF") {
      digitalWrite(LED_PIN, LOW);
      Serial.println("{\"status\":\"success\",\"led_state\":\"OFF\"}");
    }
    else if (command == "STATUS") {
      float t = dht.readTemperature();
      float h = dht.readHumidity();
      int ledState = digitalRead(LED_PIN);

      if (isnan(t) || isnan(h)) {
        Serial.println("{\"status\":\"error\",\"message\":\"DHT read failure\"}");
      } else {
        Serial.print("{\"status\":\"success\",\"temperature\":");
        Serial.print(t);
        Serial.print(",\"humidity\":");
        Serial.print(h);
        Serial.print(",\"led_state\":\"");
        Serial.print(ledState == HIGH ? "ON" : "OFF");
        Serial.println("\"}");
      }
    }
    else {
      Serial.println("{\"status\":\"error\",\"message\":\"unknown command\"}");
    }
  }
}
## 4. Python REST API Endpoints

---
### Flask REST API Running
![Flask Server Running](screenshots/flask_running.png)

### /api/health
Used to confirm that the Flask server is running correctly.
**Example Response:**
{"status": "ok"}
![API Health](screenshots/api_health.png)

### /api/status
Requests data from the Arduino.
The response includes the LED state and, if connected, the DHT22 sensor readings.
Example Response (lED only):
{"led_state": "OFF", "status": "success"}
![API Status Response](<api_status response.png>)
Example Response (when temperature and humidity are available):
{
  "status": "success",
  "temperature": 23.5,
  "humidity": 45.2,
  "led_state": "ON"
}
### api/led/on
/api/led/off

These two endpoints remotely control the LED connected to the Arduino.
The server sends a serial command to switch the LED on or off.

Example Response:{"status": "success", "led_state": "ON"}
 LED ON (/api/led/on)
![LED ON](screenshots/Led_on.png)
LED OFF (/api/led/off)
![LED OFF](screenshots/Led_off.png)

### Successful Arduino Upload
![Arduino Upload](screenshots/arduino_upload.png)