

/*this version uses Software Serial to communicate between 8266 targets and includes a HopCount
Supervisor (communicates with PC) accepts: Target,R,G,B from PC, and adds HopCount for inter-target comms
Serial data format: HopCount,Target,R,G,B  (e.g. 1001,2,200,200,100)

 * Using D1 mini, and a commercially bought 12V LED Floodlight
 * map serial input from 0-255 to 0-1023 since 8266 uses this as pwm range.
*/

// DEVICE code (not supervisor)

#include <SoftwareSerial.h>
#include "message_class.h"


// pins for software serial interface
const int serialRxPin = 4;  // white wire
const int serialTxPin = 5;  // blue wire


// create an instance of the message class to handle rgb messages: (redPin, greenPin, bluePin)
message RGB (13,12,14);
// create an instance of the message class to acknowledge messages to PC (no pins defined)
message ACK ();
// create serial interface for interconnecting 8266s (leaves Serial available for uploading and debugging)
SoftwareSerial mySerial(serialRxPin, serialTxPin); // RX, TX  


// create A pointer to Serial/mySerial to make it easier to switch for debugging
/********************************************************************************************
SELECT ONE OR OTHER SERIAL PORT TO RECEIVE INPUT DATA: SERIAL FOR DEBUGGING; MYSERIAL FOR USE
*********************************************************************************************/
//SoftwareSerial &activeSerial = mySerial;  // use for normal operation
//HardwareSerial &activeSerial = Serial;    // use for debug


void setup() {
  Serial.begin(115200);               // hardware Serial is used for firmware updates and debugging
  mySerial.begin(57600);              // Software serial is used for I/O (use lower speed since cables could be a few feet long)

  pinMode(LED_BUILTIN, OUTPUT);       // LED provides a visual que that data is being processed
  digitalWrite (LED_BUILTIN, HIGH);

  RGB.writeRGB(0,0,0);         // start with all LED's off
}


void loop() {
  // Serial data format: HopCount,Target,R,G,B  (e.g. 1002,2,200,200,100)
  // if there's  SoftwareSerial available, read it:
  if (mySerial.available()) {
    digitalWrite (LED_BUILTIN, LOW);    // LED provides a visual que that data is being processed

    int readInt = mySerial.parseInt();    // use parseint so that we read any number of digits
    char delimiter = mySerial.read();     // catch character following int value
    Serial.printf("loop: read mySerial input: %d%c\n", readInt, delimiter);

    if (readInt > 999){                 // hopcount starts at 1000, so if hopcount then increment
      readInt++;        
    }

    // echo the input to PC if avaialable, else back out to the next floodlight
    if (Serial){
      Serial.printf("mySerial sent: %d%c\n",readInt,delimiter);    
    } else {
      mySerial.printf("%d%c",readInt,delimiter);
      Serial.printf("mySerial sent: %d%c\n",readInt,delimiter);   
    }
    
    if (delimiter == '\n'){               // look for newline character
      // tell rgbMessage the we reached message end
      RGB.endMessage(readInt);  
    } else if (delimiter == ','){                          // look for value delimiter ","
        // tell rgbMessage to store incoming data
        RGB.buildMessage(readInt);
    }  

    digitalWrite (LED_BUILTIN, HIGH);
  }

  
  // ***********************************************************************
  // **** NOTE: for floodlights, need to add hopcount as first integer *****
  // ***********************************************************************
  // Serial data format: HopCount,Target,R,G,B  (e.g. 1002,2,200,200,100)
  // if there's any serial available, read it:
  if (Serial.available()) {
    digitalWrite (LED_BUILTIN, LOW);    // LED provides a visual que that data is being processed

    int readInt = Serial.parseInt();    // use parseint so that we read any number of digits
    char delimiter = Serial.read();     // catch character following int value
    Serial.printf("loop: read Serial input: %d%c\n", readInt, delimiter);

    if (readInt > 999){                 // hopcount starts at 1000, so if hopcount then increment
      readInt++;        
    }

    // echo the input back out to the next floodlight
    if (mySerial){
      mySerial.printf("%d%c",readInt,delimiter);
      Serial.printf("loop: mySerial sent: %d%c\n",readInt,delimiter);    
    } else {
      Serial.printf("loop: port not ready, mySerial = ");
      Serial.println(mySerial);
    }
    
    if (delimiter == '\n'){               // look for newline character
      // tell rgbMessage the we reached message end
      ACK.endMessage(readInt);  
    } else if (delimiter == ','){                          // look for value delimiter ","
        // tell rgbMessage to store incoming data
        ACK.buildMessage(readInt);
    }  
    digitalWrite (LED_BUILTIN, HIGH);
  }
}
