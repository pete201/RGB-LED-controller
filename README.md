# RGB-LED-controller
RGB control for commercially bought and modified 10W LED

The idea is to have a Python GUI sending serial data to a:
  Central Arduino Controller that sends RGB data via I2C to:
    Individual Arduino controlled RGB LED lamps.

I have lamps connected to 12V with a D1mini inside.  
TURN ON 12V BEFORE CONNECTING USB!! (I didn't put a diode after the 5V regulator)
Control via Serial port on Arduino IDE.  0,0,0 is all off.  255,255,255 is all on

****************************************************************************************
PROBLEM - i found in some cases that uploading from ArduinoIDE work where VSC does not!!
****************************************************************************************

I seem to have a problem in IDE where i am reading BOTH branches of a file!
i get error - such and such int already defined - it's trying to read effectively same file twice
workaround is save in IDE as a different filename - i used IDE_RGB_SUPERVISOR
NO - it still picks up other file from somewhere!!!
it copies acrss the 'testing.ino' file too
deleting 'testing.ino' solved the problem
