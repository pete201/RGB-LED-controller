import time
import re
import serial
from random import randint

ser = serial.Serial('COM5', 115200, timeout=0.1)

delay = 1
times = 10

for i in range(times):
    print(i+1)
    if i % 3 == 0:
        light = 0
        red = 255
        green = 0
        blue = 0
        ser.write(f"{light},{red},{green},{blue}\n".encode())
    if i % 3 == 1:
        light = 0
        red = 0
        green = 255
        blue = 0
        ser.write(f"{light},{red},{green},{blue}\n".encode())
    if i % 3 == 2:
        light = 0
        red = 0
        green = 0
        blue = 255
        ser.write(f"{light},{red},{green},{blue}\n".encode())
    time.sleep(delay)

light = 1
red = 255
green = 0
blue = 255
ser.write(f"{light},{red},{green},{blue}\n".encode())
light = 2
red = 0
green = 255
blue = 255
ser.write(f"{light},{red},{green},{blue}\n".encode())

ser.close()