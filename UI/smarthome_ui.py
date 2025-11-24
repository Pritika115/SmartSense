"""
SmartSense - Python Console UI (Check-In 2)
Team: SmartSense

This UI demonstrates:
- MQTT-style telemetry: Continuous temperature data reception
- REST API-style control: LED on/off commands
"""

import serial
import time
import json
import csv
from datetime import datetime
import os
import threading

class SmartSenseUI:
    def __init__(self, port, baudrate=9600):
        """
        Initialize the SmartSense UI

        Args:
            port: COM port for Arduino (e.g., 'COM3' on Windows, '/dev/ttyACM0' on Linux)
            baudrate: Serial communication speed (default 9600)
        """
        self.port = port
        self.baudrate = baudrate
        self.serial_connection = None
        self.running = False
        self.data_file = "sensor_data.csv"

        # Initialize CSV file
        self.init_data_file()

    def init_data_file(self):
        """Initialize CSV file for storing sensor data"""
        if not os.path.exists(self.data_file):
            with open(self.data_file, 'w', newline='') as f:
                writer = csv.writer(f)
                writer.writerow(['Timestamp', 'Temperature (C)', 'Humidity (%)', 'Device'])

    def connect(self):
        """Establish serial connection to Arduino"""
        try:
            self.serial_connection = serial.Serial(self.port, self.baudrate, timeout=1)
            time.sleep(2)  # Wait for Arduino to reset
            print(f"✓ Connected to Arduino on {self.port}")
            return True
        except Exception as e:
            print(f"✗ Connection failed: {e}")
            print(f"\nMake sure:")
            print(f"  1. Arduino is connected via USB")
            print(f"  2. Port is correct (e.g., 'COM3' not just '3')")
            print(f"  3. Arduino Serial Monitor is CLOSED")
            return False

    def disconnect(self):
        """Close serial connection"""
        if self.serial_connection and self.serial_connection.is_open:
            self.serial_connection.close()
            print("✓ Disconnected from Arduino")

    def read_telemetry(self):
        """
        Background thread to read MQTT-style telemetry data
        This simulates continuous sensor data streaming via MQTT
        """
        while self.running:
            if self.serial_connection and self.serial_connection.is_open:
                try:
                    if self.serial_connection.in_waiting > 0:
                        line = self.serial_connection.readline().decode('utf-8').strip()

                        if line.startswith("TELEMETRY:"):
                            # Parse JSON telemetry data
                            json_str = line.replace("TELEMETRY:", "")
                            try:
                                data = json.loads(json_str)
                                self.process_telemetry(data)
                            except json.JSONDecodeError:
                                print(f"Invalid JSON: {json_str}")

                        elif line.startswith("RESPONSE:"):
                            # Parse REST API response
                            json_str = line.replace("RESPONSE:", "")
                            try:
                                response = json.loads(json_str)
                                self.process_response(response)
                            except json.JSONDecodeError:
                                print(f"Invalid JSON: {json_str}")

                        elif line.startswith("ERROR:"):
                            print(f"⚠ {line}")

                        else:
                            # Other messages
                            if line:
                                print(f"[Arduino] {line}")

                except Exception as e:
                    print(f"Error reading telemetry: {e}")

            time.sleep(0.1)

    def process_telemetry(self, data):
        """
        Process and display telemetry data (MQTT-style)
        Also saves data to CSV file
        """
        temp = data.get('temperature', 'N/A')
        humidity = data.get('humidity', 'N/A')
        device = data.get('device', 'unknown')

        # Display in console
        print(f"\n[TELEMETRY] Temp: {temp}°C | Humidity: {humidity}% | Device: {device}")

        # Save to CSV
        timestamp = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
        with open(self.data_file, 'a', newline='') as f:
            writer = csv.writer(f)
            writer.writerow([timestamp, temp, humidity, device])

    def process_response(self, response):
        """Process REST API response"""
        status = response.get('status', 'unknown')

        if status == 'success':
            action = response.get('action', response.get('led_state', 'unknown'))
            print(f"\n[REST API] ✓ Success: {action}")
        else:
            message = response.get('message', 'Unknown error')
            print(f"\n[REST API] ✗ Error: {message}")

    def send_command(self, command):
        """
        Send REST API-style command to Arduino

        Args:
            command: Command string to send
        """
        if self.serial_connection and self.serial_connection.is_open:
            try:
                self.serial_connection.write(f"{command}\n".encode('utf-8'))
                time.sleep(0.1)
            except Exception as e:
                print(f"Error sending command: {e}")
        else:
            print("✗ Not connected to Arduino")

    def led_on(self):
        """Turn LED ON via REST API-style command"""
        print("\n[REST API] Sending: LED ON")
        self.send_command("LED:ON")

    def led_off(self):
        """Turn LED OFF via REST API-style command"""
        print("\n[REST API] Sending: LED OFF")
        self.send_command("LED:OFF")

    def get_status(self):
        """Get current system status via REST API-style command"""
        print("\n[REST API] Requesting system status")
        self.send_command("STATUS")

    def display_menu(self):
        """Display main menu"""
        print("\n" + "="*50)
        print("SmartSense Control Panel")
        print("="*50)
        print("1. Turn LED ON (REST API)")
        print("2. Turn LED OFF (REST API)")
        print("3. Get System Status (REST API)")
        print("4. View Recent Data Summary")
        print("5. Exit")
        print("="*50)
        print("\nNote: Temperature data streams automatically (MQTT)\n")

    def view_data_summary(self):
        """Display summary of recent sensor data"""
        try:
            with open(self.data_file, 'r') as f:
                reader = csv.reader(f)
                rows = list(reader)

                if len(rows) <= 1:
                    print("\nNo data collected yet.")
                    return

                # Get last 5 readings
                recent = rows[-5:] if len(rows) > 5 else rows[1:]

                print("\n" + "="*50)
                print("Recent Sensor Data (Last 5 Readings)")
                print("="*50)
                for row in recent:
                    if row[0] != 'Timestamp':  # Skip header
                        print(f"{row[0]} | Temp: {row[1]}°C | Humidity: {row[2]}%")

                # Calculate averages
                if len(rows) > 1:
                    temps = [float(row[1]) for row in rows[1:] if row[1] != 'N/A']
                    humids = [float(row[2]) for row in rows[1:] if row[2] != 'N/A']

                    if temps and humids:
                        avg_temp = sum(temps) / len(temps)
                        avg_humid = sum(humids) / len(humids)
                        print(f"\nAverages: Temp: {avg_temp:.1f}°C | Humidity: {avg_humid:.1f}%")
                        print(f"Total readings: {len(temps)}")

                print("="*50)

        except Exception as e:
            print(f"Error reading data: {e}")

    def run(self):
        """Main UI loop"""
        if not self.connect():
            return

        # Start telemetry reading thread (MQTT simulation)
        self.running = True
        telemetry_thread = threading.Thread(target=self.read_telemetry, daemon=True)
        telemetry_thread.start()

        print("\n✓ Telemetry monitoring started (MQTT-style streaming)")
        time.sleep(1)

        try:
            while True:
                self.display_menu()
                choice = input("Enter your choice (1-5): ").strip()

                if choice == '1':
                    self.led_on()
                elif choice == '2':
                    self.led_off()
                elif choice == '3':
                    self.get_status()
                elif choice == '4':
                    self.view_data_summary()
                elif choice == '5':
                    print("\nShutting down...")
                    break
                else:
                    print("\n✗ Invalid choice. Please select 1-5.")

                time.sleep(0.5)

        except KeyboardInterrupt:
            print("\n\nInterrupted by user")

        finally:
            self.running = False
            time.sleep(0.5)
            self.disconnect()


if __name__ == "__main__":
    print("="*50)
    print("SmartSense - IoT Monitoring System")
    print("Team: SmartSense")
    print("="*50)

    # Prompt user for COM port
    print("\nCommon COM ports:")
    print("  Windows: COM3, COM4, COM5, etc.")
    print("  Mac/Linux: /dev/ttyACM0, /dev/ttyUSB0, etc.")

    port = input("\nEnter Arduino COM port: ").strip().upper()

    # Auto-format COM port for Windows
    if port.isdigit():
        port = f"COM{port}"
        print(f"Auto-formatted to: {port}")

    # Create and run UI
    ui = SmartSenseUI(port)
    ui.run()
