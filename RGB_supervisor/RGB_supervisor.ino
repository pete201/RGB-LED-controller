/* NOTE - UPLOAD FROM ARDUINO, NOT VSC.  included in VSC for reference only
 *  
 * This sketch is RGB Floodlight Supervisor
 * all it does is:
 *     echo serial data from PC to first device in daisy-chain
 *     adds a hopCount int infront of PC data:
*       data in: Target,R,G,B
 *      data out: HopCount,Target,R,G,B* 
 *     
 *     waits for response from last in daisy chain
 *     if data is corrupted then send again.
 * 
 * Use Serial for comms with PC and SoftwareSerial for comms with next device
 * Main reason for this is that i can't physically fit a USB connector into my floodlights!
 */

#include <SoftwareSerial.h>
// pins for software serial interface
const int serialRxPin = 4;  // (D2) blue wire
const int serialTxPin = 5;  // (D1) white wire

int hopCount = 1001;
int target;
int red;
int green;
int blue;

int lastMillis = 0;

SoftwareSerial mySerial(serialRxPin, serialTxPin); // RX, TX  

void setup() {
  Serial.begin(115200);
  
  // Software serial is used for I/O
  mySerial.begin(57600);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite (LED_BUILTIN, LOW);
}

void loop() {
  //read data from PC
  while (Serial.available() > 0) {
    digitalWrite (LED_BUILTIN, LOW);
    Serial.print("receiving data...");

    target = Serial.parseInt();
    red = Serial.parseInt();
    green = Serial.parseInt();
    blue = Serial.parseInt();

    Serial.print(Serial.available());
    // look for the newline:
    if (Serial.read() == '\n'){
      // send data to next target, adding hopCount
      if (mySerial){
        mySerial.printf("H%d,%d,%d,%d,%d\n", hopCount, target, red, green, blue);
        // echo data back to PC 
        Serial.printf("H%d,%d,%d,%d,%d\n", hopCount, target, red, green, blue);
      } else {
        Serial.println("loop: mySerial not available");
      }
      digitalWrite (LED_BUILTIN, HIGH);
    }

    // see if we are alive
    if (millis() - lastMillis > 1000){
      Serial.print("alive");
    }
  }
  // when message circles round floodlights and gets back to supervisor, echo back to PC
  // while (mySerial.available() > 0) {

  //   // look for valid integers in the incoming serial stream; identified as 'round robin' ints
  //   int rrhopCount  = mySerial.parseInt();
  //   int rrtarget = mySerial.parseInt();
  //   int rrred = mySerial.parseInt();
  //   int rrgreen = mySerial.parseInt();
  //   int rrblue = mySerial.parseInt();

  //   // look for the newline and send back to PC
  //   if (mySerial.read() == '\n'){
  //     Serial.printf("H%d,%d,%d,%d,%d\n", rrhopCount, rrtarget, rrred, rrgreen, rrblue);
  //   } else {
  //     Serial.println("No round robin");
  //   }
  //}
}