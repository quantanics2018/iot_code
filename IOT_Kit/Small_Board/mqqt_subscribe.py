import mysql.connector
import paho.mqtt.client as mqttClient
import json
from concurrent.futures import ThreadPoolExecutor

class Mqtt:
    def __init__(self):
        self.db = mysql.connector.connect(
            host="localhost",
            user="root",
            password="",
            database="iot_db"
        )
        self.mqttclient = mqttClient.Client("2cfc106f-ac45-4dcf-9d6a-bd49ff4a615b")
        self.mqttclient.on_connect = self.on_connect
        self.mqttclient.on_message = self.on_message
        self.mqttclient.username_pw_set(username="", password="")
        mqttstatus = self.mqttclient.connect("broker.emqx.io", 1883, 60)
        self.mqttclient.subscribe("/quantanics/industry/Small_Board", 0)  # Change the topic here
        self.executor = ThreadPoolExecutor(max_workers=10)  # Adjust the number of workers as needed
        self.mqttclient.loop_forever()

    def upload(self, msg):
        try:
            mqtt_msg = msg.payload.decode("utf-8")
            sensor_data = json.loads(mqtt_msg)

            humidity = sensor_data.get("humidity", 0)
            temperature = sensor_data.get("temperature", 0)
            distance1 = sensor_data.get("distance1", 0)
            distance2 = sensor_data.get("distance2", 0)
            smoke_val = sensor_data.get("smokeValue", 0)
            ldr_val = sensor_data.get("ldrValue", 0)

            mycursor = self.db.cursor()
            sql = "INSERT INTO `small_kit`(`humidity`, `temperature`, `distance1`, `distance2`, `smoke_val`, `ir_val`) VALUES (%s,%s,%s,%s,%s,%s)"
            value_sensor = (humidity, temperature, distance1, distance2, smoke_val, ldr_val)
            mycursor.execute(sql, value_sensor)
            self.db.commit()

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
