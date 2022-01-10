/*
USE WITH TESTFLOODLIGHT.PY

this version uses USB Serial to communicate to as single 8266 target to test lamp
Serial data format: HopCount,Target,R,G,B  (e.g. 1,2,200,200,100)

 * Using D1 mini, and a commercially bought 12V LED Floodlight
 * map serial input from 0-255 to 0-1023 since 8266 uses this as pwm range.
*/
//#include <SoftwareSerial.h>


// DEVICE code (not supervisor)

// pins for the LEDs:
const int redPin = 13;
const int greenPin = 12;
const int bluePin = 14;
// pins for software serial interface
//const int serialRxPin = 4;  // white wire
//const int serialTxPin = 5;  // blue wire

// variables
int hopCount;           // counts the number of times message is relayed
int target;             // the device a serial message is addressed to
int red;                // led's
int green;
int blue;

//SoftwareSerial mySerial(serialRxPin, serialTxPin); // RX, TX  

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
  // hardware Serial is used for this test script
  //Serial.begin(115200);

  // Software serial is used for I/O (use lower speed since cables could be a few feet long)
  //mySerial.begin(57600);

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  //debug
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite (BUILTIN_LED, LOW);
  delay(500);
  digitalWrite (BUILTIN_LED, HIGH);

  writeRGB(0,255,0);  // start colour
}


void loop() {
  // if there's any serial available, read it:
  // while (Serial.available() > 0) {

  //   digitalWrite (BUILTIN_LED, LOW);

  //   // look for valid integers in the incoming serial stream:
  //   hopCount  = mySerial.parseInt();
  //   target = mySerial.parseInt();
  //   red = mySerial.parseInt();
  //   green = mySerial.parseInt();
  //   blue = mySerial.parseInt();

  //   // look for the newline:
  //   if (mySerial.read() == '\n'){

  //     // send data back to PC, increasing hopCount by 1
  //     Serial.printf("%d,%d,%d,%d,%d\n", hopCount+1,target, red, green, blue);
      
  //     if (target == hopCount || target == 0){
  //       // set LEDs on this device
  //       writeRGB(red, green, blue);
  //     }
  //   }
  //   digitalWrite (BUILTIN_LED, HIGH); 
  // }

  digitalWrite (BUILTIN_LED, LOW);
  writeRGB(255, 255, 255);
  delay(1000);
  
  digitalWrite (BUILTIN_LED, HIGH);
  writeRGB(0, 0, 0);
  delay(1000);
}
