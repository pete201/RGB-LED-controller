from time import sleep
import tkinter as tk
import tkinter.ttk as ttk
from typing import Match
from tkcolorpicker import askcolor
import re
import serial
from random import randint

try:
    ser = serial.Serial('COM5', 115200, timeout=0.1)
except: 
    print ('No serial port detected')
    exit()



# set all lights off to begin
ser.write(f"0,0,0,0\n".encode())

print('Choose either: exit, scene, every10, random or picker')
command = input('Please Choose: ')

if command == 'exit':
    ser.close()
    root.destroy()
    root.mainloop()
    exit()

elif command == 'random':
    light = input('Select a light: ')
    red = randint(0,255)
    blue = randint(0,255)
    green = randint(0,255)

    ser.write(f"{light},{red},{green},{blue}\n".encode())
    
    print(red,green,blue)
    print('')

elif command == 'scene':
    scene = input('Choose which scene: ')
    scene = open(f'Scene{scene}.txt','r')
    lines = scene.readlines()
    light1 = lines[0]
    light2 = lines[1]

    light1 = light1.replace("\n","")

    ser.write(f"{light1}\n".encode())
    print(light1)
    sleep(0.1)

    ser.write(f"{light2}\n".encode())
    print(light2)
    print('')

elif command == 'every10':
    light = 0
    loops = int(input('select number of loops: '))
    while loops > 0:
        red = randint(0,255)
        blue = randint(0,255)
        green = randint(0,255)
        ser.write(f"{light},{red},{green},{blue}\n".encode())
        
        print(red,green,blue)
        print('')
        sleep(10)

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

    red = res[0]
    green = res[1]
    blue = res[2]

    ser.write(f"{light},{red},{green},{blue}\n".encode())
    sleep(0.1)
    print(red,green,blue)
    print('')

    root.destroy()
    root.mainloop()

ser.close()


