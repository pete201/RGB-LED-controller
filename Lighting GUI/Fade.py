import time
import re
import serial
from random import randint
from playsound import playsound

def rgb(light,red,green,blue):
    ser.write(f"{light},{red},{green},{blue}\n".encode())
    print(light,red,green,blue)

ser = serial.Serial('COM5', 115200, timeout=0.1)

r = 255
g = 0
b = 0

rate = 5
delay = .1

rgb(0,r,g,b)

while True:
    if r > 0 and b == 0:
        r -= rate
        g += rate
    if g > 0 and r == 0:
        g -= rate
        b += rate
    if b > 0 and g == 0:
        r += rate
        b -= rate
    if r<0:
        r=0
    if r>255:
        r=255
    if g<0:
        g=0
    if g>255:
        g=255
    if b<0:
        b=0
    if b>255:
        b=255
    rgb(0, r, g, b)
    time.sleep(delay)