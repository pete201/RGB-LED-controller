from time import sleep
from tkinter import *
from tkinter import font
import re
import serial
from random import randint
from tkinter.colorchooser import askcolor

ser = serial.Serial('COM5', 115200, timeout=0.1)

def rgb(light,red,green,blue):
    ser.write(f"{light},{red},{green},{blue}\n".encode())
    print(light,red,green,blue)

def readScene(scene):
    if scene == "off":
        rgb(0,0,0,0)
    if scene == "white":
        rgb(0,255,255,255)
    if scene == "red":
        rgb(0,255,0,0)
    if scene == "green":
        rgb(0,0,255,0)
    if scene == "blue":
        rgb(0,0,0,255)
    if scene == "yellow":
        rgb(0,255,255,0)
    if scene == "cyan":
        rgb(0,0,255,255)
    if scene == "magenta":
        rgb(0,255,0,255)
    if scene == 1:
        rgb(1,255,0,255)
        rgb(2,0,255,255)
    if scene == 2:
        rgb(1,0,255,255)
        rgb(2,50,255,50)
    if scene == "police":
        rgb(1,255,0,0)
        rgb(2,0,0,255)
    if scene == "policeinv":
        rgb(1,0,0,255)
        rgb(2,255,0,0)

def pickColour():
    light = int(lightInput.get())
    color = askcolor("#F00")

    color = str(color)

    temp = re.findall(r'\d+', color)
    res = list(map(int, temp))

    red = res[0]
    green = res[1]
    blue = res[2]

    rgb(light, red, green, blue)

root = Tk()
root.title("Lighting GUI")
# root.iconbitmap("./icon.ico")

# create a gui with a button
label = Label(root,text="Lighting GUI").grid(row=0,column=0)
lightInput = Entry(root)
lightInput.grid(row=1,column=0)
colourButton = Button(root, text="Colour Picker", command=pickColour, width=25, height=3).grid(row=2,column=0)
blackout = Button(root, text="Black Out", command=lambda: readScene("off"), width=25, height=3, bg="#000", fg="#FFF").grid(row=3,column=0)
white = Button(root, text="White", command=lambda: readScene("white"), width=25, height=3, bg="#FFF").grid(row=4,column=0)
red = Button(root, text="Red", command=lambda: readScene("red"), width=25, height=3, bg="#F00").grid(row=5,column=0)
green = Button(root, text="Green", command=lambda: readScene("green"), width=25, height=3, bg="#0F0").grid(row=6,column=0)
blue = Button(root, text="Blue", command=lambda: readScene("blue"), width=25, height=3, bg="#00F").grid(row=7,column=0)
yellow = Button(root, text="Yellow", command=lambda: readScene("yellow"), width=25, height=3, bg="#FF0").grid(row=8,column=0)
cyan = Button(root, text="Cyan", command=lambda: readScene("cyan"), width=25, height=3, bg="#0FF").grid(row=9,column=0)
magenta = Button(root, text="Magenta", command=lambda: readScene("magenta"), width=25, height=3, bg="#F0F").grid(row=10,column=0)

scene1 = Button(root, text="Scene 1", command=lambda: readScene(1), width=25, height=3, bg="#FF00BE").grid(row=0,column=1)
scene2 = Button(root, text="Scene 2", command=lambda: readScene(2), width=25, height=3, bg="#00A0FF").grid(row=1,column=1)
police = Button(root, text="Police", command=lambda: readScene("police"), width=25, height=3, bg="#00F").grid(row=2,column=1)
policeinv = Button(root, text="Police Invert", command=lambda: readScene("policeinv"), width=25, height=3, bg="#F00").grid(row=3,column=1)

mainloop()