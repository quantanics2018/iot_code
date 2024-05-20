import mysql.connector
import paho.mqtt.client as mqttClient
from concurrent.futures import ThreadPoolExecutor

class Mqtt:
    def __init__(self):
        self.db = mysql.connector.connect(
            host="localhost",
            user="root",
            password="quantanics123",
            database="iot_db"
        )
        self.mqttclient = mqttClient.Client("2cfc106f-ac45-4dcf-9d6a-bd49ff4a615b")
        self.mqttclient.on_connect = self.on_connect
        self.mqttclient.on_message = self.on_message
        self.mqttclient.username_pw_set(username="", password="")
        mqttstatus = self.mqttclient.connect("broker.emqx.io", 1883, 60)
        self.mqttclient.subscribe("Sensor", 0)  # Change the topic here
        self.executor = ThreadPoolExecutor(max_workers=10)  # Adjust the number of workers as needed
        self.mqttclient.loop_forever()

    def upload(self, msg):
        try:
            mqtt_msg = msg.payload.decode("utf-8")
            sensor_Arr = mqtt_msg.split(",")
            
            print("Received message:", type(mqtt_msg))
            print(sensor_Arr[0])
            mycursor = self.db.cursor()
            sql="INSERT INTO `small_kit`(`humidity`, `temperature`, `distance1`, `distance2`, `smoke_val`, `ir_val`) VALUES (%s,%s,%s,%s,%s,%s)"
            value_sensor = (sensor_Arr[0],sensor_Arr[1],sensor_Arr[2],sensor_Arr[3],sensor_Arr[4],sensor_Arr[5])
            mycursor.execute(sql,value_sensor)
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
        # humdity , temperature , distance1,distance2,smoke,irvalue

if __name__ == "__main__":
    Mqtt()
