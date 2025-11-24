import paho.mqtt.client as mqtt

BROKER = "192.168.2.81"
PORT = 1883
TOPIC = "smartsense/dht22/data"

def on_connect(client, userdata, flags, rc):
    print("Connected:", rc)
    client.subscribe(TOPIC)

def on_message(client, userdata, msg):
    print("Received:", msg.payload.decode())

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect(BROKER, PORT)
client.loop_forever()
