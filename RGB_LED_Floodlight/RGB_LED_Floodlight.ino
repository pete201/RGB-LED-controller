/*this version uses Software Serial to communicate between 8266 devices.
Serial data format: DEVICE,R,G,B  (e.g. 2,200,200,100)

 * Using D1 mini, and a commercially bought 12V LED Floodlight
 *  - reverse polarity from LEDs since 0=ON
 * 
 * map serial input from 0-255 to 0-1023 since 8266 uses this as pwm range.
*/

#include <SoftwareSerial.h>

// this device address
// note address 0 is all devices
const int thisDevice = 2;
// pins for the LEDs:
const int redPin = 13;
const int greenPin = 12;
const int bluePin = 14;
// pins for software serial interface
const int serialRxPin = 4;  // white wire
const int serialTxPin = 5;  // blue wire

int device;             // the device a serial message is addressed to
int red;                // led's
int green;
int blue;

SoftwareSerial mySerial(serialRxPin, serialTxPin); // RX, TX  

void writeRGB(int r, int g, int b)
{
  // constrain the values to 0 - 255
  r = constrain(r, 0, 255);
  g = constrain(g, 0, 255);
  b = constrain(b, 0, 255);

  // map from 0-255 to 0-1023 since 8266 uses this as pwm range.
  // this is not required for Arduino which uses pwm range 0-255
  r = map(r, 0, 255, 0, 1023); // max range 1023.  choose less to limit I through LED
  g = map(g, 0, 255, 0, 1023);
  b = map(b, 0, 255, 0, 1023);

  // fade the red, green, and blue legs of the LED:
  analogWrite(redPin, r);
  analogWrite(greenPin, g);
  analogWrite(bluePin, b);
}

void setup() {
  // hardware Serial is used for input (Master uses it to talk to PC)
  Serial.begin(115200);

  // Software serial is used for OUTPUT
  mySerial.begin(115200);

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  writeRGB(0,0,0);  // start with all LED's off
}


void loop() {
  // if there's any serial available, read it:
  while (Serial.available() > 0) {

    // look for valid integers in the incoming serial stream:
    device = Serial.parseInt();
    red = Serial.parseInt();
    green = Serial.parseInt();
    blue = Serial.parseInt();
    // look for the newline:
    if (Serial.read() == '\n'){

      if (device != thisDevice){
        // send data to next device
        mySerial.printf("%d,%d,%d,%d\n", device, red, green, blue);
      }
      
      if (device == thisDevice || device == 0){
        // set LEDs on this device
        writeRGB(red, green, blue);
      }
    }
  }
}
