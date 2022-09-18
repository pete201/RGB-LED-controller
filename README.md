# RGB-LED-controller
RGB control for commercially bought and modified 10W LED

The idea is to have a Python GUI sending serial data to a:
  Central Arduino Controller that sends RGB data via I2C to:
    Individual Arduino controlled RGB LED lamps.

I have lamps connected to 12V with a D1mini inside.  
TURN ON 12V BEFORE CONNECTING USB!! (I didn't put a diode after the 5V regulator)
Control via Serial port on Arduino IDE.  0,0,0 is all off.  255,255,255 is all on

link to Miro board with wiring diagram: https://miro.com/app/board/uXjVOdfEJW8=/
