import tkinter
import serial
import time

ser = serial.Serial('COM5', 115200, timeout=0.1)

redValue = 0
greenValue = 0
blueValue = 0

def rgb(light,red,green,blue):
    ser.write(f"{light},{red},{green},{blue}\n".encode())
    print(light,red,green,blue)
    time.sleep(.03)

def set_from_sliders_red(val):
    global redValue
    redValue = val
    set_from_sliders_update()
def set_from_sliders_green(val):
    global greenValue
    greenValue = val
    set_from_sliders_update()
def set_from_sliders_blue(val):
    global blueValue
    blueValue = val
    set_from_sliders_update()
def set_from_sliders_update():
    global redValue
    global greenValue
    global blueValue
    rgb(0, redValue, greenValue, blueValue)

root = tkinter.Tk()
# root.title("Sliders")
# root.iconbitmap("./icon.ico")

red = tkinter.Scale(orient='horizontal', from_=0, to=255, command=set_from_sliders_red, length=400)
red.pack()
green = tkinter.Scale(orient='horizontal', from_=0, to=255, command=set_from_sliders_green, length=400)
green.pack()
blue = tkinter.Scale(orient='horizontal', from_=0, to=255, command=set_from_sliders_blue, length=400)
blue.pack()

root.mainloop()