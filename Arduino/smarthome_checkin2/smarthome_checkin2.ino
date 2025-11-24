/*
 * SmartSense - Check-In 2
 * TEAM MEMBER: Jeremiah
 * Arduino Uno + LED Control
 *
 * This sketch communicates with the Python REST API.
 * It responds in valid JSON format that Python can parse directly.
 * Features: Controls LED via serial commands and provides system/LED status.
 */

#define LED_PIN 7

//  SETUP
void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);

  // Send startup JSON (no DHT sensor required)
  Serial.println("{\"system\":\"started\"}");
}

// MAIN LOOP
void loop() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    // LED ON 
    if (command == "LED:ON") {
      digitalWrite(LED_PIN, HIGH);
      Serial.println("{\"status\":\"success\",\"led\":\"ON\"}");
    }

    // LED OFF 
    else if (command == "LED:OFF") {
      digitalWrite(LED_PIN, LOW);
      Serial.println("{\"status\":\"success\",\"led\":\"OFF\"}");
    }

    //  SYSTEM STATUS 
    else if (command == "STATUS") {
      int ledState = digitalRead(LED_PIN);

      Serial.print("{\"status\":\"success\",\"led_state\":\"");
      Serial.print(ledState == HIGH ? "ON" : "OFF");
      Serial.println("\"}");
    }

    // UNKNOWN COMMAND
    else {
      Serial.println("{\"status\":\"error\",\"message\":\"Unknown command\"}");
    }
  }
}
