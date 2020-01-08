#ifndef Button_h
#define Button_h

#include "Arduino.h"

class Button
{
public:
  Button(int pin);
  int read();

private:
  int _pin;
  int _lastValue;
  unsigned long _lastDebounceTime;
};

#endif
