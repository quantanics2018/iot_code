import mysql.connector
from mysql.connector import pooling
import paho.mqtt.client as mqttClient
from threading import Thread
import random
import json

class Mqtt:
    def __init__(self):
        self.db_config = {
            "host": "localhost",
            "user": "root",
            "password": "",
            "database": "iot_db"
        }
        self.db_pool = mysql.connector.pooling.MySQLConnectionPool(
            pool_name="mypool", pool_size=5, **self.db_config)
        
        client_id = f'python-mqtt-{random.randint(0, 1000)}'
        self.mqttclient = mqttClient.Client(mqttClient.CallbackAPIVersion.VERSION1, client_id)
        self.mqttclient.on_connect = self.on_connect
        self.mqttclient.on_message = self.on_message
        self.mqttclient.username_pw_set(username="", password="")
        mqttstatus = self.mqttclient.connect("broker.emqx.io", 1883, 60)
        self.mqttclient.subscribe("quantanics/industry/soc_soh_data", 0) 
        self.mqttclient.loop_forever()

    def get_db_connection(self):
        return self.db_pool.get_connection()

    def check_connection(self, db_connection):
        if not db_connection.is_connected():
            print("Reconnecting to MySQL...")
            db_connection.reconnect(attempts=3, delay=5)

    def Calculate_soc(self, voltage):
        values = [
            (10.50, 0),
            (11.40, 10), 
            (12.00, 20),
            (12.60, 30),
            (13.20, 40),
            (13.80, 50),
            (14.40, 60),
            (15.00, 70),
            (15.60, 80),
            (16.20, 90), 
            (16.80, 100)
        ]
        print("calculate soc working fine")

        for i in range(len(values) - 1):
            v1, p1 = values[i]
            v2, p2 = values[i + 1]
            if v1 <= voltage <= v2:  
                percentage = p1 + (voltage - v1) * (p2 - p1) / (v2 - v1)
                return percentage

        if voltage >= 13.00:
            return 100
        elif voltage <= 10.50:
            return 0

    def measured_capacity (self, discharge_current, discharge_time):
        capacity = discharge_current * discharge_time
        capacity_mah = capacity * 1000
        return self.calculate_soh(capacity_mah,8000)

    def calculate_soh(self, measured_voltage, rated_voltage):
        if rated_voltage <= 0:
            print("Rated voltage Must be Higher than 0 ")

        soh = (measured_voltage/rated_voltage) * 100
        return soh
    

    def upload(self, msg):
        try:  
            mqtt_msg = msg.payload.decode("utf-8")
            print(mqtt_msg)
            
            sensor_data = json.loads(mqtt_msg)
            voltage = sensor_data["voltage"]
            current = sensor_data["current"]
            soc = self.Calculate_soc(voltage)
            soh = self.measured_capacity(0.5,15.5)
            print(soc)
            print(current)
            
            db_connection = self.get_db_connection() 
            self.check_connection(db_connection)  
            
            cursor = db_connection.cursor()
            sql = "INSERT INTO soc_soh_tb (`voltage`, `soc`, `current`, `soh`) VALUES (%s,%s,%s,%s)"
            val = (voltage, soc, current, soh)
            cursor.execute(sql, val)
            db_connection.commit()
            print("Data Inserted!")
            cursor.close()
            db_connection.close()  

        except mysql.connector.Error as err:
            print("MySQL Error:", err)
        except Exception as e:
            print("Error:", e)

    def on_connect(self, mqttclient, userdata, flags, rc):
        if rc == 0:
            print("Connected to MQTT broker!")
        else:
            print("Connection to MQTT broker failed")

    def on_message(self, mqttclient, userdata, msg):
        Thread(target=self.upload, args=(msg,)).start()

if __name__ == "__main__":
    Mqtt()
