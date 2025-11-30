"""
SmartSense MQTT Subscriber
Receives temperature and humidity data from ESP32
"""

import paho.mqtt.client as mqtt
import json
from datetime import datetime
import requests
import time

# MQTT Configuration
MQTT_BROKER = "broker.hivemq.com"
MQTT_PORT = 1883
MQTT_TOPIC_TEMP = "smartsense/temperature"
MQTT_TOPIC_HUMIDITY = "smartsense/humidity"

# ESP32 REST API Configuration
ESP32_IP = "192.168.0.62"  # Replace with your ESP32 IP address
REST_API_BASE = f"http://{ESP32_IP}"

# Storage for latest readings
latest_data = {
    'temperature': None,
    'humidity': None,
    'last_update': None
}


def on_connect(client, userdata, flags, rc):
    """Callback when connected to MQTT broker"""
    if rc == 0:
        print("✓ Connected to MQTT Broker!")
        client.subscribe(MQTT_TOPIC_TEMP)
        client.subscribe(MQTT_TOPIC_HUMIDITY)
        print(f"✓ Subscribed to topics:")
        print(f"  - {MQTT_TOPIC_TEMP}")
        print(f"  - {MQTT_TOPIC_HUMIDITY}")
        print("\n" + "="*50)
        print("Waiting for sensor data...")
        print("="*50 + "\n")
    else:
        print(f"✗ Failed to connect, return code {rc}")


def on_message(client, userdata, msg):
    """Callback when message is received"""
    topic = msg.topic
    payload = msg.payload.decode()
    
    if topic == MQTT_TOPIC_TEMP:
        latest_data['temperature'] = float(payload)
    elif topic == MQTT_TOPIC_HUMIDITY:
        latest_data['humidity'] = float(payload)
    
    latest_data['last_update'] = datetime.now()
    
    # Display the data
    display_data()


def display_data():
    """Display the latest sensor readings"""
    if latest_data['temperature'] is not None and latest_data['humidity'] is not None:
        timestamp = latest_data['last_update'].strftime("%Y-%m-%d %H:%M:%S")
        
        print(f"[{timestamp}]")
        print(f"  🌡️  Temperature: {latest_data['temperature']:.1f}°C")
        print(f"  💧 Humidity:    {latest_data['humidity']:.1f}%")
        print("-" * 50)


def control_led(state):
    """Control LED via REST API"""
    try:
        url = f"{REST_API_BASE}/led"
        payload = {"state": state}
        response = requests.post(url, json=payload, timeout=5)
        
        if response.status_code == 200:
            print(f"\n✓ LED turned {state.upper()}")
            print(f"Response: {response.json()}\n")
        else:
            print(f"\n✗ Failed to control LED: {response.text}\n")
    except Exception as e:
        print(f"\n✗ Error controlling LED: {e}\n")


def get_system_status():
    """Get full system status via REST API"""
    try:
        url = f"{REST_API_BASE}/status"
        response = requests.get(url, timeout=5)
        
        if response.status_code == 200:
            data = response.json()
            print("\n" + "="*50)
            print("SYSTEM STATUS")
            print("="*50)
            print(f"Temperature:  {data.get('temperature', 'N/A')}°C")
            print(f"Humidity:     {data.get('humidity', 'N/A')}%")
            print(f"LED State:    {data.get('led_state', 'N/A').upper()}")
            print(f"WiFi Signal:  {data.get('wifi_rssi', 'N/A')} dBm")
            print(f"IP Address:   {data.get('ip_address', 'N/A')}")
            print("="*50 + "\n")
        else:
            print(f"\n✗ Failed to get status: {response.text}\n")
    except Exception as e:
        print(f"\n✗ Error getting status: {e}\n")


def print_menu():
    """Print the user menu"""
    print("\n" + "="*50)
    print("SMARTSENSE CONTROL PANEL")
    print("="*50)
    print("1. Turn LED ON")
    print("2. Turn LED OFF")
    print("3. Get System Status")
    print("4. Exit")
    print("="*50)


def main():
    """Main function"""
    print("\n")
    print("╔══════════════════════════════════════════════════╗")
    print("║          SmartSense IoT Monitoring System        ║")
    print("╚══════════════════════════════════════════════════╝")
    print()
    
    # Create MQTT client
    client = mqtt.Client(client_id="SmartSense_Python_Client")
    client.on_connect = on_connect
    client.on_message = on_message
    
    # Connect to broker
    try:
        print(f"Connecting to MQTT broker at {MQTT_BROKER}:{MQTT_PORT}...")
        client.connect(MQTT_BROKER, MQTT_PORT, 60)
    except Exception as e:
        print(f"✗ Connection failed: {e}")
        return
    
    # Start MQTT loop in background
    client.loop_start()
    
    # Interactive menu
    try:
        while True:
            print_menu()
            choice = input("Enter your choice (1-4): ").strip()
            
            if choice == "1":
                control_led("on")
            elif choice == "2":
                control_led("off")
            elif choice == "3":
                get_system_status()
            elif choice == "4":
                print("\nExiting SmartSense Control Panel...")
                break
            else:
                print("\n✗ Invalid choice. Please try again.\n")
            
            time.sleep(0.5)
    
    except KeyboardInterrupt:
        print("\n\nInterrupted by user")
    
    finally:
        client.loop_stop()
        client.disconnect()
        print("✓ Disconnected from MQTT broker")
        print("\nGoodbye!\n")


if __name__ == "__main__":
    main()
