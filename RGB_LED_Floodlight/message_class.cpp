// message_class.h
#include "message_class.h"
#include <Arduino.h>


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
  //Serial.printf("buildMessage: adding data %d to array position %d\n", data, arrayIndex);
  messageArray[arrayIndex] = data;
  arrayIndex++;
}

void message::endMessage(int data) {
  // first call buildMessage to add last data element to array
  buildMessage(data);
  // if we have correct number of data elements, set LEDs
  if (arrayIndex == 5){
    if (messageArray[target] == messageArray[hopCount]-1001 || messageArray[target] == 0){
          // set LEDs on this device
          writeRGB(messageArray[red],messageArray[green],messageArray[blue]);
          Serial.printf("endMessage: %d,%d,%d,%d,%d\n\n", messageArray[hopCount], messageArray[target], messageArray[red], messageArray[green], messageArray[blue]);
    } else {
      Serial.println("endMessage: message not for this LED");
    }
  } else {
    Serial.print("endMessage: incorrect buffer size: ");       // if wrong array size then do nothing
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
