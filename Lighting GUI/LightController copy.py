from time import sleep
import json
import tkinter as tk
import tkinter.ttk as ttk
from typing import Match
from tkcolorpicker import askcolor
import re
import serial
from random import randint

# get serial port from arduino.json file
MyFile = open('.vscode/arduino.json')
MyData = json.load(MyFile)
MyPort = MyData['port']
MyFile.close()

# open serial port to arduino
try:
    ser = serial.Serial(port=MyPort, baudrate=115200, timeout=.1)
except: 
    print ('No serial port detected')
    exit()
ser.reset_input_buffer


print('Choose either: exit, scene, every10, random or picker')
#command = input('Please Choose: ')
command = 'random'

if command == 'exit':
    ser.close()
    exit()

elif command == 'random':
    light = input('Select a light: ')
    red = randint(0,255)
    green = randint(0,255)
    blue = randint(0,255)    

    ser.write(f"{light},{red},{green},{blue}\n".encode())
    
    print("Output values: ",red,green,blue)

    sleep(0.5) # necessary to wait for arduino to respond

    if ser.in_waiting > 0:
        data = str(ser.readline())
        print("data received: ", data)
    else:
        print("No serial data")


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


