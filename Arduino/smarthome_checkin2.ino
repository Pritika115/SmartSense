/*
 * SmartSense - Check-In 2
 * Team: SmartSense
 * Hardware: Arduino Uno + DHT22 + LED
 *
 * Automatic MQTT-style telemetry streaming
 */

#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT22
#define LED_PIN 13  // Changed to Pin 13 for built-in LED support

DHT dht(DHTPIN, DHTTYPE);

unsigned long previousMillis = 0;
const long interval = 2000;

void setup() {
  Serial.begin(9600);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  dht.begin();

  Serial.println("SmartSense System Started");
  Serial.println("Ready for commands");
}

void loop() {
  unsigned long currentMillis = millis();

  // Auto-send telemetry every 2 seconds (MQTT pattern)
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    readAndSendSensorData();
  }

  // Handle REST API commands
  if (Serial.available() > 0) {
    handleCommand();
  }
}

void readAndSendSensorData() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("ERROR:Failed to read from DHT sensor");
    return;
  }

  Serial.print("TELEMETRY:{\"type\":\"temperature\",\"device\":\"arduino_uno\",\"temperature\":");
  Serial.print(temperature, 1);
  Serial.print(",\"humidity\":");
  Serial.print(humidity, 1);
  Serial.print(",\"unit\":\"C\",\"timestamp\":");
  Serial.print(millis());
  Serial.println("}");
}

void handleCommand() {
  String command = Serial.readStringUntil('\n');
  command.trim();

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
    int ledState = digitalRead(LED_PIN);
    Serial.print("RESPONSE:{\"status\":\"success\",\"led_state\":\"");
    Serial.print(ledState == HIGH ? "ON" : "OFF");
    Serial.println("\"}");
  }
  else {
    Serial.println("RESPONSE:{\"status\":\"error\",\"message\":\"Unknown command\"}");
  }
}
