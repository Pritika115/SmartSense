/*
 * SmartSense - Check-In 2
 * Team: SmartSense
 * Hardware: Arduino Uno + DHT22 + LED
 *
 * Integrated DHT22 sensor for temperature/humidity monitoring
 */

#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT22
#define LED_PIN 7

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.println("SmartSense System Started");
  Serial.println("Ready for commands");
}

void loop() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command == "LED:ON") {
      digitalWrite(LED_PIN, HIGH);
      Serial.println("RESPONSE:{\"status\":\"success\",\"action\":\"LED_ON\",\"device\":\"arduino_uno\"}");
    }
    else if (command == "LED:OFF") {
      digitalWrite(LED_PIN, LOW);
      Serial.println("RESPONSE:{\"status\":\"success\",\"action\":\"LED_OFF\",\"device\":\"arduino_uno\"}");
    }
    else if (command == "STATUS") {
      int ledState = digitalRead(LED_PIN);
      Serial.print("RESPONSE:{\"status\":\"success\",\"led_state\":\"");
      Serial.print(ledState == HIGH ? "ON" : "OFF");
      Serial.println("\"}");
    }
    else if (command == "TEMP") {
      float h = dht.readHumidity();
      float t = dht.readTemperature();

      if (!isnan(h) && !isnan(t)) {
        Serial.print("TELEMETRY:{\"type\":\"temperature\",\"device\":\"arduino_uno\",\"temperature\":");
        Serial.print(t, 1);
        Serial.print(",\"humidity\":");
        Serial.print(h, 1);
        Serial.print(",\"unit\":\"C\",\"timestamp\":");
        Serial.print(millis());
        Serial.println("}");
      } else {
        Serial.println("ERROR:Failed to read from DHT sensor");
      }
    }
    else {
      Serial.println("RESPONSE:{\"status\":\"error\",\"message\":\"Unknown command\"}");
    }
  }
}
