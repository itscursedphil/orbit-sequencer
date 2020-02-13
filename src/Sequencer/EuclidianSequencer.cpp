#include "Arduino.h"
#include "EuclidianSequencer.h"

EuclidianSequencer::EuclidianSequencer()
{
  for (int i = 0; i < 16; i++)
  {
    _pattern[i] = 0;
  }

  _pattern[0] = 1;

  _mode = 0;
  _length = 16;
  _steps = 1;
  _offset = 0;
  _masterTimer = 0;
  _timer = 0;

  generateEuclidian();
}

int *EuclidianSequencer::getPattern()
{
  return _pattern;
}

int EuclidianSequencer::getMode()
{
  return _mode;
}

int EuclidianSequencer::setMode(int mode)
{
  _mode = mode;
}

void EuclidianSequencer::toggleMode()
{
  if (_mode == 0)
  {
    _mode = 1;
  }
  else
  {
    _mode = 0;
  }
}

int EuclidianSequencer::getLength()
{
  return _length;
}

int EuclidianSequencer::setLength(int length)
{
  _length = length;

  if (_mode == 0)
  {
    generateEuclidian();
  }
}

void EuclidianSequencer::incLength()
{
  if (_length < 16)
  {
    _length = _length + 1;
    // Sync with master clock
    _timer = _masterTimer % (_length * 8); // n Steps (Pattern length) * 4 Ratchets * 2 (Gate on/off)

    if (_mode == 0)
    {
      generateEuclidian();
    }
  }
}

void EuclidianSequencer::decLength()
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

    if (_mode == 0)
    {
      generateEuclidian();
    }
  }
}

int EuclidianSequencer::getSteps()
{
  return _steps;
}

int EuclidianSequencer::setSteps(int steps)
{
  _steps = steps;

  generateEuclidian();
}

void EuclidianSequencer::incSteps()
{
  if (_steps < _length)
  {
    _steps = _steps + 1;

    generateEuclidian();
  }
}

void EuclidianSequencer::decSteps()
{
  if (_steps > 0)
  {
    _steps = _steps - 1;

    generateEuclidian();
  }
}

int EuclidianSequencer::getOffset()
{
  return _offset;
}

int EuclidianSequencer::setOffset(int offset)
{
  _offset = offset;

  generateEuclidian();
}

void EuclidianSequencer::incOffset()
{
  _offset = (_offset + 1) % _length;

  generateEuclidian();
}

void EuclidianSequencer::decOffset()
{
  _offset = (_offset + _length - 1) % _length;

  generateEuclidian();
}

int EuclidianSequencer::getOffsetIndex(int index)
{
  return (index + _offset) % _length;
}

void EuclidianSequencer::incTimer()
{
  _masterTimer = (_masterTimer + 1) % 128; // 16 Steps * 4 Ratchets * 2 (Gate on/off)
  _timer = (_timer + 1) % (_length * 8);   // n Steps (Pattern length) * 4 Ratchets * 2 (Gate on/off)
}

int EuclidianSequencer::getTimer()
{
  return _timer;
}

void EuclidianSequencer::generateEuclidian()
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