/*
 * SmartSense IoT Project
 * ESP32 with DHT22 sensor and LED control
 * - MQTT: Publishes temperature/humidity data
 * - REST API: Controls LED state
 */

#include <WiFi.h>
#include <PubSubClient.h>
#include <WebServer.h>
#include <DHT.h>
#include <ArduinoJson.h>

// WiFi credentials
const char* ssid = "Seeteria";
const char* password = "seeteria";

// MQTT Broker settings
const char* mqtt_server = "broker.hivemq.com";  // Free public broker
const int mqtt_port = 1883;
const char* mqtt_topic_temp = "smartsense/temperature";
const char* mqtt_topic_humidity = "smartsense/humidity";
const char* mqtt_client_id = "SmartSense_ESP32";

// DHT22 settings
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// LED settings
#define LED_PIN 18
bool ledState = false;

// Web server on port 80
WebServer server(80);

// MQTT client
WiFiClient espClient;
PubSubClient mqtt(espClient);

// Timing
unsigned long lastMqttPublish = 0;
const long mqttInterval = 5000;  // Publish every 5 seconds

void setup() {
  Serial.begin(115200);
  
  // Initialize LED
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  // Initialize DHT sensor
  dht.begin();
  
  // Connect to WiFi
  connectWiFi();
  
  // Setup MQTT
  mqtt.setServer(mqtt_server, mqtt_port);
  
  // Setup REST API endpoints
  server.on("/", HTTP_GET, handleRoot);
  server.on("/led", HTTP_GET, handleLedGet);
  server.on("/led", HTTP_POST, handleLedPost);
  server.on("/status", HTTP_GET, handleStatus);
  server.onNotFound(handleNotFound);
  
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  // Ensure MQTT connection
  if (!mqtt.connected()) {
    reconnectMQTT();
  }
  mqtt.loop();
  
  // Handle REST API requests
  server.handleClient();
  
  // Publish sensor data via MQTT
  unsigned long currentMillis = millis();
  if (currentMillis - lastMqttPublish >= mqttInterval) {
    lastMqttPublish = currentMillis;
    publishSensorData();
  }
}

void connectWiFi() {
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println();
  Serial.println("WiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnectMQTT() {
  while (!mqtt.connected()) {
    Serial.print("Attempting MQTT connection...");
    
    if (mqtt.connect(mqtt_client_id)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqtt.state());
      Serial.println(" retrying in 5 seconds");
      delay(5000);
    }
  }
}

void publishSensorData() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();  // Celsius
  
  // Check if readings are valid
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  // Create JSON payload
  StaticJsonDocument<200> doc;
  doc["temperature"] = temperature;
  doc["humidity"] = humidity;
  doc["timestamp"] = millis();
  
  char buffer[200];
  serializeJson(doc, buffer);
  
  // Publish to MQTT
  mqtt.publish(mqtt_topic_temp, String(temperature).c_str());
  mqtt.publish(mqtt_topic_humidity, String(humidity).c_str());
  
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print("°C | Humidity: ");
  Serial.print(humidity);
  Serial.println("%");
}

// REST API Handlers
void handleRoot() {
  String html = "<html><body>";
  html += "<h1>SmartSense IoT System</h1>";
  html += "<p>Temperature & Humidity monitoring with LED control</p>";
  html += "<h2>API Endpoints:</h2>";
  html += "<ul>";
  html += "<li>GET /status - Get system status</li>";
  html += "<li>GET /led - Get LED state</li>";
  html += "<li>POST /led - Control LED (body: {\"state\":\"on\"} or {\"state\":\"off\"})</li>";
  html += "</ul>";
  html += "</body></html>";
  
  server.send(200, "text/html", html);
}

void handleStatus() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  
  StaticJsonDocument<300> doc;
  doc["temperature"] = temperature;
  doc["humidity"] = humidity;
  doc["led_state"] = ledState ? "on" : "off";
  doc["wifi_rssi"] = WiFi.RSSI();
  doc["ip_address"] = WiFi.localIP().toString();
  
  String response;
  serializeJson(doc, response);
  
  server.send(200, "application/json", response);
}

void handleLedGet() {
  StaticJsonDocument<100> doc;
  doc["led_state"] = ledState ? "on" : "off";
  
  String response;
  serializeJson(doc, response);
  
  server.send(200, "application/json", response);
}

void handleLedPost() {
  if (server.hasArg("plain")) {
    String body = server.arg("plain");
    
    StaticJsonDocument<100> doc;
    DeserializationError error = deserializeJson(doc, body);
    
    if (error) {
      server.send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
      return;
    }
    
    String state = doc["state"];
    
    if (state == "on") {
      ledState = true;
      digitalWrite(LED_PIN, HIGH);
      server.send(200, "application/json", "{\"led_state\":\"on\",\"message\":\"LED turned on\"}");
      Serial.println("LED turned ON via REST API");
    } else if (state == "off") {
      ledState = false;
      digitalWrite(LED_PIN, LOW);
      server.send(200, "application/json", "{\"led_state\":\"off\",\"message\":\"LED turned off\"}");
      Serial.println("LED turned OFF via REST API");
    } else {
      server.send(400, "application/json", "{\"error\":\"Invalid state. Use 'on' or 'off'\"}");
    }
  } else {
    server.send(400, "application/json", "{\"error\":\"No body provided\"}");
  }
}

void handleNotFound() {
  server.send(404, "application/json", "{\"error\":\"Endpoint not found\"}");
}
