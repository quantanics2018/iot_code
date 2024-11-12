import mysql.connector
import paho.mqtt.client as mqttClient
import json
from concurrent.futures import ThreadPoolExecutor
import random

class Mqtt:
    def __init__(self):
        self.db = mysql.connector.connect(
            host="localhost",
            user="root",
            password="",
            database="iotkit"
        )
        client_id = f'python-mqtt-{random.randint(0, 1000)}'
        # self.mqttclient = mqttClient.Client(client_id)
        self.mqttclient = mqttClient.Client(mqttClient.CallbackAPIVersion.VERSION1, client_id)
        self.mqttclient.on_connect = self.on_connect
        self.mqttclient.on_message = self.on_message
        self.mqttclient.username_pw_set(username="", password="")
        mqttstatus = self.mqttclient.connect("broker.emqx.io", 1883, 60)
        self.mqttclient.subscribe("quantanics/industry/agri", 0)  # Change the topic here
        self.executor = ThreadPoolExecutor(max_workers=10)  # Adjust the number of workers as needed
        self.mqttclient.loop_forever()

    def upload(self, msg):
        try:
            mqtt_msg = msg.payload.decode("utf-8")
            print("received message is ")
            print(mqtt_msg)
            sensor_data = json.loads(mqtt_msg)

            # humidity = sensor_data.get("humidity", 0)
            # temperature = sensor_data.get("temperature", 0)
            # distance1 = sensor_data.get("distance1", 0)
            # distance2 = sensor_data.get("distance2", 0)
            # ldr_val = sensor_data.get("ldrValue", 0)
            """ humidity = sensor_data['humidity']
            temperature = sensor_data['temperature']

            mycursor = self.db.cursor()
            sql = "INSERT INTO dht11_tb(temperature, humidity) VALUES (%s,%s)"
            value_sensor = (temperature,humidity)
            mycursor.execute(sql, value_sensor)
            self.db.commit()  """

            print(sensor_data)

            print("Data Inserted!")
        except Exception as e:
            print("Error:", e)

    def on_connect(self, mqttclient, userdata, flags, rc):
        if rc == 0:
            print("Connected to MQTT broker!")
        else:
            print("Connection to MQTT broker failed")

    def on_message(self, mqttclient, userdata, msg):
        self.executor.submit(self.upload, msg)

if __name__ == "__main__":
    Mqtt()