# RGB-LED-controller
RGB control for commercially bought and modified 10W LED

The idea is to have a Python GUI sending serial data to a:
  Central Arduino Controller that sends RGB data via serial to:
    Individual Arduino controlled RGB LED lamps.

I have lamps connected to 12V with an 8266-D1mini inside.  
TURN ON 12V BEFORE CONNECTING USB!! (On early models I didn't put a diode after the 5V regulator)

Python talks to Supervisor via serial interface, sending message:
ID      the number of the light to control, or 0 for all lights
Red     value 0-255
Green   value 0-255
Blue    value 0-255

the Supervisor prepends a 'hopcount' to the Python message.  A hopcount is used so that all lamps have the same code
Rather than hard code an ID for each lamp, the hopcount is incremented each time the message is passed on
each lamp then compares the hopcount with the desired lamp ID, and when they match the correct lamp is controlled

The hopcount value is deliberately outside of the valid range of all other elements so that it can also act as a message start

---------------------------------------
idea
to make supervisor and leds similar, i could have USB and soft-serial 'message' objects
Supervisor routes hard->soft and soft->hard
Leds route soft->soft, unless hard is active, in which case it acts same as supervisor (h->s and s->h for debugging)
  supervisor always has to act same way, but lights change message routing if USB is connected.

for a universal controller with USB and ext serial ports:
if data comes in from USB, send it out on ext
if data comes in from ext, first try USB, else send on ext.

or perhaps we can establish a 'status' 
staus default = lamp
if (valid message received on USB) then status = supervisor
... lamp connects ext to ext
... supervisor connects USB->ext and ext->USB.

but otherwise identical; so set up message objects for both USB and ext, just define routing depending on status.
this way if we get noise on the USB port for example, it won't just leap into working like a supervisor.

having said that, i kind of think the USB staus will be quite dependable

