from flask import Flask, jsonify
import serial
import threading
import json
import time

SERIAL_PORT = "COM8"
BAUD_RATE = 9600
SERIAL_TIMEOUT = 2

app = Flask(__name__)

serial_lock = threading.Lock()
ser = None

def get_serial():
    global ser
    if ser is None or not ser.is_open:
        ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=SERIAL_TIMEOUT)
        time.sleep(2)
    return ser

def send_command(command: str):
    with serial_lock:
        s = get_serial()
        s.reset_input_buffer()
        s.reset_output_buffer()
        s.write((command + "\n").encode())
        line = s.readline().decode(errors="ignore").strip()
    try:
        return json.loads(line)
    except:
        return {"response": line, "status": "raw"}

@app.route("/api/health")
def health():
    return {"status": "ok"}

@app.route("/api/led/on", methods=["POST"])
def led_on():
    return jsonify(send_command("LED:ON"))

@app.route("/api/led/off", methods=["POST"])
def led_off():
    return jsonify(send_command("LED:OFF"))

@app.route("/api/status")
def status():
    return jsonify(send_command("STATUS"))

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000)
