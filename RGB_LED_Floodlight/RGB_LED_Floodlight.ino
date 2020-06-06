/*this version tries Software Serial to communicate between 8266 devices.
Serial data format: DEVICE,R,G,B  (e.g. 2,200,200,100)


 * v04 is a branch from v03 for a Floodlight hack
 * Using D1 mini, and a commercially bought 12V LED Floodlight
 *  - remove 200mA limit
 *  - reverse polarity from LEDs since 0=ON
 * 
 * v03 compile for 'D1 R2 and mimi', i am using D1 mini
 * pins were 4,5,16 on 8266 12E module, now 2,4,5
 * 
 * v02 (keep same name for now to reduce # folders
 * map serial input from 0-255 to 0-1023 since 8266 uses this as pwm range.
*/

#include <SoftwareSerial.h>

// this device address
// note address 0 is all devices
const int thisDevice = 2;
// pins for the LEDs:
const int redPin = 2;
const int greenPin = 4;
const int bluePin = 5;
// pins for software serial interface
const int serialRxPin = 12;
const int serialTxPin = 13;

int device;             // the device a serial message is addressed to
int red = 0;            // initialise with LED off
int green = 0;
int blue = 0;

SoftwareSerial mySerial(serialRxPin, serialTxPin); // RX, TX  

void setup() {
  // hardware Serial is used for input (Master uses it to talk to PC)
  Serial.begin(115200);

  // Software serial is used for OUTPUT
  mySerial.begin(115200);

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

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

      if (device == thisDevice || device == 0){
        writeRGB(red, green, blue);
      }

      if (device != thisDevice){
        // send data to next device
        mySerial.print(device); mySerial.print(",");
        mySerial.print(red); mySerial.print(",");
        mySerial.print(green); mySerial.print(",");
        mySerial.println(blue);
      }
    }
  }
}
