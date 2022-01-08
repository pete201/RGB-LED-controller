

/*this version uses Software Serial to communicate between 8266 targets and includes a HopCount
Supervisor (communicates with PC) accepts: Target,R,G,B from PC, and adds HopCount for inter-target comms
Serial data format: HopCount,Target,R,G,B  (e.g. 1,2,200,200,100)

 * Using D1 mini, and a commercially bought 12V LED Floodlight
 * 
 * map serial input from 0-255 to 0-1023 since 8266 uses this as pwm range.
*/

// DEVICE code (not supervisor)

#include <SoftwareSerial.h>

class message {
  public:
    message (int, int, int);
    void resetCounter() {arrayIndex = 0;}
    void buildMessage(int);
    void endMessage();
    void writeRGB(int, int, int);
  private:
    int redPin;
    int greenPin;
    int bluePin;
    const int hopCount = 0;           // counts the number of times message is relayed
    const int target = 1;             // the device a serial message is addressed to
    const int red = 2;                // led's
    const int green = 3;
    const int blue = 4;
    int nextData;
    int messageArray [16];            // array used to build message piece by piece
    int arrayIndex = 0;

};

message::message(int red, int green, int blue){
  // constructor for message
  redPin = red;
  greenPin = green;
  bluePin = blue;
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

void message::buildMessage(int data) {
  // on incoming serial, add next data item to array and increase counter (arrayIndex)
  Serial.println("adding data item to array...");
  messageArray[arrayIndex] = data;
  arrayIndex++;
}

void message::endMessage() {
  // if we have correct number of data elements, set LEDs
  if (arrayIndex == 5){
    if (messageArray[target] == messageArray[hopCount] || messageArray[target] == 0){
          // set LEDs on this device
          writeRGB(messageArray[red],messageArray[green],messageArray[blue]);
        }
  } else {
    Serial.print("incorrect buffer size: ");       // if wrong array size then do nothing
    Serial.println(arrayIndex);
  }
  // regardless of correct number of elements, reset counter for next message
  arrayIndex = 0;
}

void message::writeRGB(int r, int g, int b) {
  // function for setting LEDs
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

  //debug
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
    Serial.print("got serial...");
    Serial.println(in);

    if (in == 'H'){     //special case; marks beginning of message
      Serial.println("begin start sequence. found H: ");
      int hop = Serial.parseInt();   // use parseint so that we read any number of digits
      Serial.println(hop);
      // tell rgbMessage we have a start signal (resets counter)
      rgbMessage.resetCounter();
      // tell rgbMessage to store hopCount
      rgbMessage.buildMessage(hop);

      hop++;
      Serial.print("Next hop is: ");
      Serial.println(hop);          
      //mySerial.print(hop); mySerial.print(",");   // pass increased hopCount to next LED floodlight

    } else if (in == '\n'){                         // look for newline character
        // tell rgbMessage to store incoming data
        rgbMessage.buildMessage(messagePart);
        // tell rgbMessage the we reached message end
        rgbMessage.endMessage();  
    } else if (in == ','){                          // look for value delimiter ","
        // tell rgbMessage to store incoming data
        rgbMessage.buildMessage(messagePart);
    } else {                                        // we have an incoming digit
      messagePart = messagePart * 10 + in;
    }
       
    digitalWrite (BUILTIN_LED, HIGH);

// send data to next target, increasing hopCount by 1
//mySerial.printf("%d,%d,%d,%d,%d\n", hopCount,target, red, green, blue);

  }
}
      
