/*
 * SmartSense - Check-In 2
 * Team: SmartSense
 * Hardware: Arduino Uno + DHT22 + LED
 *
 * This sketch reads temperature/humidity from DHT22 sensor
 * and controls an LED via serial commands from Python UI
 */

#include <DHT.h>

// DHT22 Sensor Configuration
#define DHTPIN 2          // DHT22 data pin connected to Digital Pin 2
#define DHTTYPE DHT22     // DHT sensor type (changed from DHT11 to DHT22)
DHT dht(DHTPIN, DHTTYPE);

// LED Configuration
#define LED_PIN 13        // LED connected to Digital Pin 13

// Timing
unsigned long previousMillis = 0;
const long interval = 2000;  // Read sensor every 2 seconds

void setup() {
  // Initialize serial communication at 9600 baud
  Serial.begin(9600);
  while (!Serial) { ; }  // Wait for serial port to connect

  // Initialize LED pin
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // Initialize DHT sensor
  dht.begin();

  // AM2302/DHT22 needs time to stabilize
  delay(2000);

  // Startup message
  Serial.println("SmartSense System Started");
  Serial.println("Ready for commands");
}

void loop() {
  unsigned long currentMillis = millis();

  // Read and send temperature data at regular intervals (MQTT telemetry simulation)
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    readAndSendSensorData();
  }

  // Check for incoming commands (REST API simulation)
  if (Serial.available() > 0) {
    handleCommand();
  }
}

void readAndSendSensorData() {
  // Read temperature and humidity
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Check if readings are valid
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("ERROR:Failed to read from DHT sensor");
    return;
  }

  // Send data in JSON-like format for MQTT telemetry
  Serial.print("TELEMETRY:");
  Serial.print("{\"type\":\"temperature\",\"device\":\"arduino_uno\",\"temperature\":");
  Serial.print(temperature, 1);
  Serial.print(",\"humidity\":");
  Serial.print(humidity, 1);
  Serial.print(",\"unit\":\"C\",\"timestamp\":");
  Serial.print(millis());
  Serial.println("}");
}

void handleCommand() {
  // Read the incoming command
  String command = Serial.readStringUntil('\n');
  command.trim();

  // Parse and execute commands (REST API simulation)
  if (command.startsWith("LED:")) {
    String action = command.substring(4);
    action.trim();

    if (action == "ON") {
      digitalWrite(LED_PIN, HIGH);
      Serial.println("RESPONSE:{\"status\":\"success\",\"action\":\"LED_ON\",\"device\":\"arduino_uno\"}");
    }
    else if (action == "OFF") {
      digitalWrite(LED_PIN, LOW);
      Serial.println("RESPONSE:{\"status\":\"success\",\"action\":\"LED_OFF\",\"device\":\"arduino_uno\"}");
    }
    else {
      Serial.println("RESPONSE:{\"status\":\"error\",\"message\":\"Invalid LED command\"}");
    }
  }
  else if (command == "STATUS") {
    // Return current system status
    int ledState = digitalRead(LED_PIN);
    Serial.print("RESPONSE:{\"status\":\"success\",\"led_state\":\"");
    Serial.print(ledState == HIGH ? "ON" : "OFF");
    Serial.println("\"}");
  }
  else {
    Serial.println("RESPONSE:{\"status\":\"error\",\"message\":\"Unknown command\"}");
  }
}
