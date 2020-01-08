#ifndef Encoder_h
#define Encoder_h

#include "Arduino.h"

class Encoder
{
public:
  Encoder(int pinA, int pinB);
  int read();

private:
  int _pinA;
  int _pinB;
  int _lastValueA;
};

#endif
