import paho.mqtt.client as mqtt
import time

# MQTT broker settings
BROKER = "127.0.0.1"  # use localhost if broker runs locally
PORT = 1883
TOPIC = "test/topic"

# Callback when the client connects to the broker
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connected successfully!")
    else:
        print(f"Connection failed with code {rc}")

# Callback when a message is published
def on_publish(client, userdata, mid):
    print(f"Message {mid} published.")

# Create MQTT client and assign callbacks
client = mqtt.Client()
client.on_connect = on_connect
client.on_publish = on_publish

# Connect to broker
try:
    client.connect(BROKER, PORT, 60)
except Exception as e:
    print(f"Failed to connect: {e}")
    exit(1)

client.loop_start()  # start the network loop

# Publish messages
for i in range(5):
    message = f"Hello MQTT {i}"
    result = client.publish(TOPIC, message)
    # result.wait_for_publish()  # optional if you want synchronous publishing
    time.sleep(1)

client.loop_stop()
client.disconnect()
print("Publisher finished.")
