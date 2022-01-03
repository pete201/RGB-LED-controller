/* NOTE - UPLOAD FROM ARDUINO, NOT VSC.  included in VSC for reference only
 *  
 * This sketch is RGB Floodlight Supervisor
 * all it does is:
 *     echo serial data from PC to first device in daisy-chain
 *     adds a hopCount int infront of PC data:
*       data in: Target,R,G,B
 *      data out: HopCount,Target,R,G,B* 
 *     
 * supervisor continuously sends the latest message from Serial (Python)
 * we do this to solve the problem of messages getting lost because of our crummy mySerial data transmission
 * 
 * Use Serial for comms with PC and SoftwareSerial for comms with next device
 * Main reason for this is that i can't physically fit a USB connector into my floodlights!
 */

#include <Arduino.h>
#include <SoftwareSerial.h>
// pins for software serial interface
const int serialRxPin = 4;  // white wire
const int serialTxPin = 5;  // blue wire

int hopCount = 1;
int target = 0;
int red = 0;
int green = 100;
int blue = 200;

//SoftwareSerial mySerial(serialRxPin, serialTxPin); // RX, TX  

void setup() {
  Serial.begin(115200);
  Serial.print("started Serial");
  // Software serial is used for I/O
  //mySerial.begin(57600);
  //print("Started software serial")

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, 0);
}

void loop() {
  //read data from PC
  while (Serial.available() > 0) {

    digitalWrite(LED_BUILTIN, 0);   // turn ON built in LED

    target = Serial.parseInt();
    red = Serial.parseInt();
    green = Serial.parseInt();
    blue = Serial.parseInt();
    Serial.readString()             // flush any remaining characters until the newline:
    }
  }

  red++;
  if (red > 255){
    red=0;
  }
  green++;
  if (green>255){
    green=0;
  }
  blue++;
  if (blue>255){
    blue=0;
  }

  //mySerial.printf("%d,%d,%d,%d,%d\n", hopCount, target, red, green, blue);
  

  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));   // toggle built in LED

  delay(100);    // this delay is to give LEDs a chance to read and process messages

  Serial.println("looping")
}

