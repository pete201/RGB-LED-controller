// message_class.h

#ifndef MESSAGE_CLASS_H
#define MESSAGE_CLASS_H

class message {
  public:
    message (int, int, int);
    void resetCounter() {arrayIndex = 0;}
    void buildMessage(int);
    void endMessage(int);
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
    int messageArray [16];            // array used to build message piece by piece
    int arrayIndex = 0;
};

#endif