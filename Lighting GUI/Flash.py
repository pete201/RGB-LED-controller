import time
import re
import serial
from random import randint
from playsound import playsound

def rgb(light,red,green,blue):
    ser.write(f"{light},{red},{green},{blue}\n".encode())

ser = serial.Serial('COM5', 115200, timeout=0.1)

delay = .05

for i in range(100):
    rgb(1,255,255,255)
    rgb(2,0,0,0)
    time.sleep(delay)
    rgb(2,255,255,255)
    rgb(1,0,0,0)
    time.sleep(delay)
rgb(0,255,0,255)