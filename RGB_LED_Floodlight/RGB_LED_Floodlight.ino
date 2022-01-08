

/*this version uses Software Serial to communicate between 8266 targets and includes a HopCount
Supervisor (communicates with PC) accepts: Target,R,G,B from PC, and adds HopCount for inter-target comms
Serial data format: HopCount,Target,R,G,B  (e.g. 1,2,200,200,100)

 * Using D1 mini, and a commercially bought 12V LED Floodlight
 * 
 * map serial input from 0-255 to 0-1023 since 8266 uses this as pwm range.
*/

// DEVICE code (not supervisor)

#include <SoftwareSerial.h>
#include "message_class.h"


/***********************************************************************************************************************************
// TODO - uncomment the mySerial print lines to send data to next floodlight.  commented out as it causes hang if port not connected
***********************************************************************************************************************************/


// pins for software serial interface
const int serialRxPin = 4;  // white wire
const int serialTxPin = 5;  // blue wire
// variable for builiding up incoming integer char by char
int messagePart = 0;

// create an instance of the message class to handle rgb messages: (redPin, greenPin, bluePin)
message rgbMessage (13,12,14);
// create serial interface for interconnecting 8266s (leaves Serial available for uploading and debugging)
SoftwareSerial mySerial(serialRxPin, serialTxPin); // RX, TX  


void setup() {
  // hardware Serial is used for firmware updates only (Supervisor uses it to talk to PC)
  Serial.begin(115200);
  // Software serial is used for I/O (use lower speed since cables could be a few feet long)
  mySerial.begin(57600);

  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite (BUILTIN_LED, HIGH);

  rgbMessage.writeRGB(0,0,0);  // start with all LED's off
}


void loop() {
  // Serial data format: HopCount,Target,R,G,B  (e.g. H1,2,200,200,100)
  // if there's any serial available, read it:
  if (Serial.available()) {
    digitalWrite (BUILTIN_LED, LOW);

    // read one char from serial:
    char in = Serial.read();
    Serial.print("loop: got serial: ");
    Serial.println(in);

    if (in == 'H'){                   //special case; marks beginning of message
      Serial.println("loop: begin start sequence. found H: ");
      int hop = Serial.parseInt();    // use parseint so that we read any number of digits
      Serial.println(hop);
      // tell rgbMessage we have a start signal (resets counter)
      rgbMessage.resetCounter();

      // set messagePart to current hop - will be added to message on recipt of ","
      messagePart = hop;
      hop++;
      Serial.print("loop: Next hop is: ");
      Serial.println(hop);          
      //mySerial.printf("H%d",hop);    // pass increased hopCount to next LED floodlight

    } else {
      // in all other cases but "H", echo to mySerial
      //mySerial.printf(in);    // pass increased hopCount to next LED floodlight
      if (in == '\n'){                         // look for newline character
        // tell rgbMessage to store incoming data
        rgbMessage.buildMessage(messagePart);
        messagePart = 0;
        // tell rgbMessage the we reached message end
        rgbMessage.endMessage();  
      } else if (in == ','){                          // look for value delimiter ","
          // tell rgbMessage to store incoming data
          rgbMessage.buildMessage(messagePart);
          messagePart = 0;
      } else {                                        // we have an incoming digit
        messagePart = (messagePart * 10) + (in - 48); // convert ASCII char to integer ("1" = char 49)
      }
    }  
    digitalWrite (BUILTIN_LED, HIGH);
  }
}
      
