from time import sleep
import tkinter as tk
import tkinter.ttk as ttk
from tkcolorpicker import askcolor
import re
import serial
from random import randint

ser = serial.Serial('COM7', 115200, timeout=0.1)

root = tk.Tk()

style = ttk.Style(root)
style.theme_use('clam')

print('Choose either: scene, random or which light you are going to change')

while True:
    light = input('Please Choose: ')
    if light == 'exit':
        ser.write(f"0,0,0,0\n".encode())
        sleep(0.1)
        break
    
    if light == 'random':
        light = input('Select a light: ')
        red = randint(0,255)
        blue = randint(0,255)
        green = randint(0,255)

        ser.write(f"{light},{red},{green},{blue}\n".encode())
        sleep(0.1)
        print(red,green,blue)
        print('')
    elif light == 'scene':
        light = input('Choose which scene: ')
        scene = open(f'Scene{light}.txt','r')
        lines = scene.readlines()
        light1 = lines[0]
        light2 = lines[1]

        light1 = light1.replace("\n","")

        print(light1)
        print(light2)
        print('')

        ser.write(f"{light1}\n".encode())
        ser.write(f"{light2}\n".encode())
        sleep(0.1)
    elif light == 'every10':
        light = input('Select a light: ')
        while True:
            red = randint(0,255)
            blue = randint(0,255)
            green = randint(0,255)

            ser.write(f"{light},{red},{green},{blue}\n".encode())
            sleep(10)
            print(red,green,blue)
            print('')
    else:
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
ser.close()

root.destroy()

root.mainloop()
