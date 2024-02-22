import csv
from time import time
import serial

# Your serial port might be different!
ser = serial.Serial('/dev/cu.usbserial-14110', timeout=1)
print(ser)
f = open("ks.csv", "a+", newline='')
writer = csv.writer(f, delimiter=',')
while True:
    s = ser.readline().decode()
    if s != "":
        rows = [float(str(x).replace('\n', '')) for x in s.split(',')]
        # Insert local time to list's first position
        rows.insert(0, int(time()))
        print(rows)

        writer.writerow(rows)
        f.flush()