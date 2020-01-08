#ifndef Sequencer_h
#define Sequencer_h

#include "Arduino.h"

class Sequencer
{
public:
  Sequencer();
  int *getPattern();
  int getMode();
  int setMode(int mode);
  void toggleMode();
  int getLength();
  int setLength(int length);
  void incLength();
  void decLength();
  int getSteps();
  int setSteps(int steps);
  void incSteps();
  void decSteps();
  int getOffset();
  int setOffset(int offset);
  void incOffset();
  void decOffset();
  void euclidian();

private:
  int _pattern[16];
  int _mode;
  int _length;
  int _steps;
  int _offset;
};

#endif
