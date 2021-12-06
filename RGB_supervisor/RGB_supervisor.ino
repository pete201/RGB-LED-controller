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

#include <arduino>
#include <SoftwareSerial.h>
// pins for software serial interface
const int serialRxPin = 4;  // white wire
const int serialTxPin = 5;  // blue wire

int hopCount = 1;
int target;
int red;
int green;
int blue;

SoftwareSerial mySerial(serialRxPin, serialTxPin); // RX, TX  

void setup() {
  Serial.begin(115200);
  
  // Software serial is used for I/O
  mySerial.begin(57600);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, 1);
}

void loop() {
  //read data from PC
  while (Serial.available() > 0) {
    target = Serial.parseInt();
    red = Serial.parseInt();
    green = Serial.parseInt();
    blue = Serial.parseInt();
    // look for the newline:
    if (Serial.read() != '\n'){
      // blink the onboard LED to indicate data error
      digitalWrite(LED_BUILTIN, 0);   // turn ON built in LED
      delay(10);
      digitalWrite(LED_BUILTIN, 1);   // turn OFF built in LED
    }
  }

  mySerial.printf("%d,%d,%d,%d,%d\n", hopCount, target, red, green, blue);
  delay(10);    // this delay is to give LEDs a chance to read and process messages

}
