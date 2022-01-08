

/*this version uses Software Serial to communicate between 8266 targets and includes a HopCount
Supervisor (communicates with PC) accepts: Target,R,G,B from PC, and adds HopCount for inter-target comms
Serial data format: HopCount,Target,R,G,B  (e.g. 1,2,200,200,100)

 * Using D1 mini, and a commercially bought 12V LED Floodlight
 * 
 * map serial input from 0-255 to 0-1023 since 8266 uses this as pwm range.
*/
#include <SoftwareSerial.h>
//#include <dummy.h>

// DEVICE code (not supervisor)

// pins for the LEDs:
const int redPin = 13;
const int greenPin = 12;
const int bluePin = 14;
// pins for software serial interface
const int serialRxPin = 4;  // white wire
const int serialTxPin = 5;  // blue wire

// variables
int hopCount;           // counts the number of times message is relayed
int target;             // the device a serial message is addressed to
int red;                // led's
int green;
int blue;

int input[16];           // input buffer to store incoming serial data.  Only need 5 bytes, but extra is incase we miss some newlines.
int arrayPointer = 0;    // used to index our array


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

void buildMessage()
{
  // build message from the incoming serial if we have 5 bytes

  Serial.println("building message...");

  hopCount  = input[0];
  target = input[1];
  red = input[2];
  green = input[3];
  blue = input[4];
}

void setup() {
  // hardware Serial is used for firmware updates only (Supervisor uses it to talk to PC)
  Serial.begin(115200);

  // Software serial is used for I/O (use lower speed since cables could be a few feet long)
  mySerial.begin(57600);

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  //debug
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite (BUILTIN_LED, HIGH);

  writeRGB(0,0,0);  // start with all LED's off
}


void loop() {
  // Serial data format: HopCount,Target,R,G,B  (e.g. 1,2,200,200,100)
  // if there's any serial available, read it:
  if (Serial.available()) {
    digitalWrite (BUILTIN_LED, LOW);

    // read one char from serial:
    char in = Serial.read();
    Serial.print("got serial...");
    Serial.println(in);

    if (in == 'H'){     //special case; marks beginning of message
      Serial.println("begin start sequence. found H: ");
      hopCount = Serial.parseInt();   // use parseint so that we read any number of digits
      Serial.println(hopCount);
      // store hopCount

      hopCount++;
      Serial.print("Next hop is: ");
      Serial.println(hopCount);

    }

    //TODO read CHAR until ',', then i have an INT. etc etc

    // look for newline character
    if (in != '\n'){
      input[arrayPointer] = in;
      arrayPointer++;
    } else {
      if (arrayPointer == 4){
        buildMessage();

        if (target == hopCount || target == 0){
          // set LEDs on this device
          writeRGB(red, green, blue);
        }
        
        // send data to next target, increasing hopCount by 1
        hopCount++;
        mySerial.printf("%d,%d,%d,%d,%d\n", hopCount,target, red, green, blue);
      } else {
        Serial.print("incorrect buffer size: ");       // if wrong array size then do nothing
        Serial.println(arrayPointer);
      }
      arrayPointer = 0;  
      digitalWrite (BUILTIN_LED, HIGH);
    }
  }
}
      
