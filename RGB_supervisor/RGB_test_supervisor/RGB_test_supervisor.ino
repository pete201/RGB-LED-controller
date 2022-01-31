/* NOTE - UPLOAD FROM ARDUINO, NOT VSC.  included in VSC for reference only
 *  
 * This sketch is TEST RGB Floodlight Supervisor
 * what it does is:
 *   each 1s, send a valid message to lamp: 1001,0,r,g,b
 *      data r g b is derived from millis
 *     
 * Use SoftwareSerial for comms with next device
 */

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <cstdlib>              // for rand()
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
  lastMillis = millis();
  pinMode(LED_BUILTIN, OUTPUT);
  
  Serial.begin(115200);
  // while (!Serial){
  //   digitalWrite(LED_BUILTIN, LOW);
  // }
  // digitalWrite(LED_BUILTIN, HIGH);
  
  // Serial.println("setup:");

  // Software serial is used for I/O
  mySerial.begin(9600);

}

void loop()
{
  // each second
  if ((millis() - lastMillis) > 1000)
  {
    lastMillis = millis();
    //Serial.println("alive");
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    //Serial.println("receiving data...");

    target = 0;
    red = (rand() % 255) + 1;
    green = (rand() % 255) + 1;
    blue = (rand() % 255) + 1;


    // send data to next target, adding hopCount
    if (mySerial)
    {
      mySerial.printf("H%d,%d,%d,%d,%d\n", hopCount, target, red, green, blue);
      // echo data back to PC
      Serial.printf("%d,%d,%d,%d,%d\n", hopCount, target, red, green, blue);
    }
    else
    {
      Serial.println("loop: mySerial not available");
    }
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
