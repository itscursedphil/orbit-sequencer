#include "Arduino.h"
#include "Button.h"

Button::Button(int pin)
{
  _pin = pin;
  pinMode(_pin, INPUT);
}

int Button::read()
{
  int value = digitalRead(_pin);
  int lastValue = _lastValue;

  unsigned long debounceDelay = 50;
  bool debounced = false;

  if (value != lastValue)
  {
    _lastDebounceTime = millis();
  }

  if ((millis() - _lastDebounceTime) > debounceDelay)
  {
    debounced = true;
  }

  _lastValue = value;

  if (debounced == true)
  {
    return value;
  }
  else
  {
    return lastValue;
  }
}
