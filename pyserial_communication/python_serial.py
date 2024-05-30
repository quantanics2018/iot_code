import mysql.connector
import serial
import time

def mysql_ins(temp):
    try:
        print('Passing value:')
        print(f'Temperature: {temp}')
        
        db_config = mysql.connector.connect(
            host='localhost',
            user='root',
            password='quantanics123',
            database='iot'
        )
        
        mycursor = db_config.cursor()
        sql = "INSERT INTO it (Temperature) VALUES (%s)"
        val = (temp,)
        
        mycursor.execute(sql, val)
        db_config.commit()
        
        print("Insertion success")
    except mysql.connector.Error as err:
        print(f"Error: {err}")
    finally:
        mycursor.close()
        db_config.close()

def main():
    while True:
        try:
            my = serial.Serial("/dev/ttyUSB0", 9600, parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE, bytesize=serial.EIGHTBITS)
            print(my)
            
            line = my.readline()
            data = line.decode('utf-8')
            print(data)
            
            myarr = data.split('&&')
            print(myarr)
            
            if len(myarr) >= 1:
                temp = myarr[0].strip()
                # mysql_ins(temp)
            else:
                print("Received data is incomplete:", myarr)
            
        except serial.SerialException as e:
            print(f"Serial error: {e}")
        except Exception as e:
            print(f"Error: {e}")
        finally:
            if 'my' in locals() and my.is_open:
                my.close()
        
        time.sleep(1)  # Add a delay to avoid hammering the serial port

if __name__ == "__main__":
    main()