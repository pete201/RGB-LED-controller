#include <Arduino.h>
#include <SoftwareSerial.h>


// pins for software serial interface
const int serialRxPin = 4;  // white wire
const int serialTxPin = 5;  // blue wire

int hopCount = 1;
int target = 1000;
int red;
int green;
int blue;

SoftwareSerial mySerial(serialRxPin, serialTxPin); // RX, TX  

void setup() {
  Serial.begin(115200);
  
  // Software serial is used for I/O
  mySerial.begin(57600);

  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
 //read data from PC
 while (Serial.available() > 0) {
    digitalWrite(LED_BUILTIN,0);
    
    target = Serial.parseInt();
    red = Serial.parseInt();
    green = Serial.parseInt();
    blue = Serial.parseInt();
    Serial.readString();             // flush any remaining characters until the newline:
    
    // print new timeDelay
    Serial.print("new delay set: ");
    Serial.println(target);
    
    digitalWrite(LED_BUILTIN, 1);
  }

  mySerial.printf("%d,%d,%d,%d,%d\n", hopCount, target, red, green, blue);
  
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));   // turn the LED on (HIGH is the voltage level)
  Serial.println(target);
  delay(target);                       // wait 
}