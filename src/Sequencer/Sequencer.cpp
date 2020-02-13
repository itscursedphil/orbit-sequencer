#include "Arduino.h"
#include "Sequencer.h"

Sequencer::Sequencer()
{
  for (int i = 0; i < 16; i++)
  {
    _pattern[i] = 0;
  }

  _pattern[0] = 1;

  _length = 16;
  _steps = 1;
  _offset = 0;
  _masterTimer = 0;
  _timer = 0;

  generateEuclidian();
}

int *Sequencer::getPattern()
{
  return _pattern;
}

int Sequencer::getLength()
{
  return _length;
}

int Sequencer::setLength(int length)
{
  _length = length;
}

void Sequencer::incLength()
{
  if (_length < 16)
  {
    _length = _length + 1;
    // Sync with master clock
    _timer = _masterTimer % (_length * 8); // n Steps (Pattern length) * 4 Ratchets * 2 (Gate on/off)
  }
}

void Sequencer::decLength()
{
  if (_length > 1)
  {
    _length = _length - 1;
    // Sync with master clock
    _timer = _masterTimer % (_length * 8); // n Steps (Pattern length * 4 Ratchets * 2 (Gate on/off)

    if (_steps > _length)
    {
      _steps = _length;
    }
  }
}

int Sequencer::getSteps()
{
  return _steps;
}

int Sequencer::setSteps(int steps)
{
  _steps = steps;
}

void Sequencer::incSteps()
{
  if (_steps < _length)
  {
    _steps = _steps + 1;
  }
}

void Sequencer::decSteps()
{
  if (_steps > 0)
  {
    _steps = _steps - 1;
  }
}

void Sequencer::toggleStep(int index)
{
  int currentStepValue = _pattern[index];

  if (currentStepValue == 0)
  {
    _pattern[index] = 1;
  }
  else
  {
    _pattern[index] = 0;
  }
}

int Sequencer::getOffset()
{
  return _offset;
}

int Sequencer::setOffset(int offset)
{
  _offset = offset;
}

void Sequencer::incOffset()
{
  int currentPattern[16];

  for (int i = 0; i < 16; i++)
  {
    currentPattern[i] = _pattern[i];
  }

  for (int i = 0; i < 16; i++)
  {
    int previousStepIndex = (i - 1 + 16) % 16;

    _pattern[i] = currentPattern[previousStepIndex];
  }
}

void Sequencer::decOffset()
{
  int currentPattern[16];

  for (int i = 0; i < 16; i++)
  {
    currentPattern[i] = _pattern[i];
  }

  for (int i = 0; i < 16; i++)
  {
    int nextStepIndex = (i + 1) % 16;

    _pattern[i] = currentPattern[nextStepIndex];
  }
}

int Sequencer::getOffsetIndex(int index)
{
  return (index + _offset) % _length;
}

void Sequencer::incTimer()
{
  _masterTimer = (_masterTimer + 1) % 128; // 16 Steps * 4 Ratchets * 2 (Gate on/off)
  _timer = (_timer + 1) % (_length * 8);   // n Steps (Pattern length) * 4 Ratchets * 2 (Gate on/off)
}

int Sequencer::getTimer()
{
  return _timer;
}

void Sequencer::generateEuclidian()
{
  for (int i = 0; i < 16; i++)
  {
    _pattern[i] = 0;
  }

  int previous = -1;

  if (_steps > 1)
  {
    for (int i = 0; i < _length; i++)
    {
      int index = getOffsetIndex(i);
      int val = (_steps * 1.0) / (_length * 1.0) * i;

      if (val == previous)
      {
        _pattern[index] = 0;
      }
      else
      {
        _pattern[index] = 1;
      }

      previous = val;
    }
  }
  else if (_steps == 1)
  {
    int index = getOffsetIndex(0);
    _pattern[index] = 1;
  }
}