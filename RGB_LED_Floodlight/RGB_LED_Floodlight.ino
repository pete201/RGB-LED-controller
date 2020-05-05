/*
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
 * ______________________________________________
 * v02
 * using 8266 instead of Arduino.
 * 
 * All LEDs on: enter 0,0,0
 * All LEDs off: enter 1024,1024,1024
 * ______________________________________________
 * 
 * https://www.arduino.cc/en/Tutorial/ReadASCIIString
*/

// pins for the LEDs:
const int redPin = 2;
const int greenPin = 4;
const int bluePin = 5;

int red = 0;            // initialise with LED off
int green = 0;
int blue = 0;
  

void setup() {
  Serial.begin(115200);

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

void loop() {
  // if there's any serial available, read it:
  while (Serial.available() > 0) {

    // look for valid integers in the incoming serial stream:
    red = Serial.parseInt();
    green = Serial.parseInt();
    blue = Serial.parseInt();

    // look for the newline:
    if (Serial.read() == '\n') {
      // constrain the values to 0 - 255
      red = constrain(red, 0, 255);
      green = constrain(green, 0, 255);
      blue = constrain(blue, 0, 255);

      // map from 0-255 to 0-1023 since 8266 uses this as pwm range.
      // this is not required for Arduino which uses pwm range 0-255
      red = map(red, 0, 255, 0, 1023);     // max range 1023.  choose less to limit I through LED
      green = map(green, 0, 255, 0, 1023);
      blue = map(blue, 0, 255, 0, 1023);

      // fade the red, green, and blue legs of the LED:
      analogWrite(redPin, red);
      analogWrite(greenPin, green);
      analogWrite(bluePin, blue);

      // DEBUG print the three numbers in one string as hexadecimal:
      Serial.print(red, HEX);
      Serial.print(green, HEX);
      Serial.println(blue, HEX);
    }
  }
}
