#ifndef Sequencer_h
#define Sequencer_h

class Sequencer
{
public:
  Sequencer();

  int *getPattern();

  // MODES
  int getMode();
  int setMode(int mode);
  void toggleMode();

  // LENGTH
  int getLength();
  int setLength(int length);
  void incLength();
  void decLength();

  // STEPS
  int getSteps();
  int setSteps(int steps);
  void incSteps();
  void decSteps();
  void toggleStep(int index);

  // OFFSET
  int getOffset();
  int setOffset(int offset);
  void incOffset();
  void decOffset();
  int getOffsetIndex(int index);

  // TIMER
  void incTimer();
  int getTimer();

  // PATTERN GENERATORS
  void generateEuclidian();
  void generateRandom();

private:
  int _pattern[16];
  int _mode;
  int _length;
  int _steps;
  int _offset;
  int _masterTimer;
  int _timer;
};

#endif
