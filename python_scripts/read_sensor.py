import csv
from time import time
import serial

# Your serial port might be different!
ser = serial.Serial('com4', timeout=1)
f = open("lk_pressed.csv", "a+", newline='')
writer = csv.writer(f, delimiter=',')
while True:
    s = ser.readline().decode('utf-8')
    if s != "":
        rows = [float(str(x).replace('\n', '')) for x in s.split(',')]
        # Insert local time to list's first position
        rows.insert(0, int(time()))
        print(rows)

        writer.writerow(rows)
        f.flush() 