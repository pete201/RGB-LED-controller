"""
this test prog talks to just one floodlight directly (not via supervisor)
this version uses USB Serial to communicate to as single 8266 target to test lamp
Serial data format: HopCount,Target,R,G,B  (e.g. 1,2,200,200,100)
"""


from time import sleep
import tkinter as tk
import tkinter.ttk as ttk
from typing import Match
from tkcolorpicker import askcolor
import re
import serial
from random import randint

try:
    ser = serial.Serial('COM3', 115200, timeout=0.1)
except: 
    print ('No serial port detected')
    exit()



print('Choose either: exit, every3, random or picker')
command = input('Please Choose: ')

if command == 'exit':
    ser.close()
    exit()

elif command == 'random':
    hopcount = 1
    light = 1
    red = randint(0,255)
    blue = randint(0,255)
    green = randint(0,255)

    ser.write(f"{hopcount},{light},{red},{green},{blue}\n".encode())


    print(red,green,blue)
    print('')

elif command == 'every3':
    hopcount = 1
    light = 1
    loops = int(input('select number of loops: '))
    while loops > 0:
        red = randint(0,255)
        green = randint(0,255)
        blue = randint(0,255)
        
        ser.write(f"{hopcount},{light},{red},{green},{blue}\n".encode())
        
        print(red,green,blue)
        print('')
        #sleep(10)
        sleep(3)

        loops = loops - 1

elif command == 'picker':
    light = input('Select a light: ')

    print('Select a hue from the colour picker and click OK')

    #set up TK for tkcolorpicker
    root = tk.Tk()
    style = ttk.Style(root)
    style.theme_use('clam')

    color = askcolor((255, 255, 255), root)
    color = str(color)

    temp = re.findall(r'\d+', color)
    res = list(map(int, temp))

    hopcount = 1
    light = 1
    red = res[0]
    green = res[1]
    blue = res[2]

    ser.write(f"{hopcount},{light},{red},{green},{blue}\n".encode())
    sleep(0.1)
    print(red,green,blue)
    print('')

    root.destroy()
    root.mainloop()

ser.close()


