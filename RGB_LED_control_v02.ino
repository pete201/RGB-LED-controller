/*
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
const int redPin = 4;
const int greenPin = 5;
const int bluePin = 16;

void setup() {
  // initialize serial:
  Serial.begin(115200);
  // make the pins outputs:
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

}

void loop() {
  // if there's any serial available, read it:
  while (Serial.available() > 0) {

    // look for valid integers in the incoming serial stream:
    int red = Serial.parseInt();
    int green = Serial.parseInt();
    int blue = Serial.parseInt();

    // look for the newline:
    if (Serial.read() == '\n') {
      // constrain the values to 0 - 255
      red = 255 - constrain(red, 0, 255);
      green = 255 - constrain(green, 0, 255);
      blue = 255 - constrain(blue, 0, 255);

      // map from 0-255 to 0-1023 since 8266 uses this as pwm range.
      // this is not required for Arduino which uses pwm range 0-255
      red = map(red, 0, 255, 0, 1023);
      green = map(green, 0, 255, 0, 1023);
      blue = map(blue, 0, 255, 0, 1023);

      // fade the red, green, and blue legs of the LED:
      analogWrite(redPin, red);
      analogWrite(greenPin, green);
      analogWrite(bluePin, blue);

      // print the three numbers in one string as hexadecimal:
      Serial.print(red, HEX);
      Serial.print(green, HEX);
      Serial.println(blue, HEX);
    }
  }
}
