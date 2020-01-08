#include "Arduino.h"
#include "Encoder.h"

Encoder::Encoder(int pinA, int pinB)
{
  _pinA = pinA;
  _pinB = pinB;
  pinMode(_pinA, INPUT);
  pinMode(_pinB, INPUT);
}

int Encoder::read()
{
  int valueA = digitalRead(_pinA);
  int valueB = digitalRead(_pinB);
  int value = 0;

  if (_lastValueA == 0 && valueA == 1 && valueB == 0)
  {
    value = -1;
  }
  else if (_lastValueA == 1 && valueA == 0 && valueB == 0)
  {
    value = 1;
  }

  _lastValueA = valueA;

  return value;
}
