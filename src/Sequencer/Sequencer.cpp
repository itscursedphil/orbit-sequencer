#include "Arduino.h"
#include "Sequencer.h"

Sequencer::Sequencer()
{
  for (int i = 0; i < 16; i++)
  {
    _pattern[i] = 0;
  }

  _mode = 0;
  _length = 16;
  _steps = 1;
  _offset = 0;

  euclidian();
}

int *Sequencer::getPattern()
{
  return _pattern;
}

int Sequencer::getMode()
{
  return _mode;
}

int Sequencer::setMode(int mode)
{
  _mode = mode;
}

void Sequencer::toggleMode()
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

int Sequencer::getLength()
{
  return _length;
}

int Sequencer::setLength(int length)
{
  _length = length;

  if (_mode == 0)
  {
    euclidian();
  }
}

void Sequencer::incLength()
{
  if (_length < 16)
  {
    _length = _length + 1;

    if (_mode == 0)
    {
      euclidian();
    }
  }
}

void Sequencer::decLength()
{
  if (_length > 1)
  {
    _length = _length - 1;

    if (_steps > _length)
    {
      _steps = _length;
    }

    if (_mode == 0)
    {
      euclidian();
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

  euclidian();
}

void Sequencer::incSteps()
{
  if (_steps < _length)
  {
    _steps = _steps + 1;

    euclidian();
  }
}

void Sequencer::decSteps()
{
  if (_steps > 0)
  {
    _steps = _steps - 1;

    euclidian();
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
  _offset = (_offset + 1) % 16;
}

void Sequencer::decOffset()
{
  _offset = (_offset + 16 - 1) % 16;
}

void Sequencer::euclidian()
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
      int val = (_steps * 1.0) / (_length * 1.0) * i;

      if (val == previous)
      {
        _pattern[i] = 0;
      }
      else
      {
        _pattern[i] = 1;
      }

      previous = val;
    }
  }
  else if (_steps == 1)
  {
    _pattern[0] = 1;
  }
}