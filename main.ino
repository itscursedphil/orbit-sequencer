#include <TimerOne.h>
#include <Adafruit_NeoPixel.h>
#include "src/Encoder/Encoder.h"
#include "src/Button/Button.h"
#include "src/Sequencer/Sequencer.h"

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(16, 22, NEO_GRB + NEO_KHZ800);

uint32_t cBlue = pixels.Color(0, 0, 255);
uint32_t cBlueDark = pixels.Color(0, 0, 15);
uint32_t cRed = pixels.Color(255, 0, 0);
uint32_t cRedDark = pixels.Color(15, 0, 0);
uint32_t cGreen = pixels.Color(0, 255, 0);
uint32_t cGreenDark = pixels.Color(0, 15, 0);
uint32_t cActiveStep = pixels.Color(255, 255, 255);
uint32_t cInactiveStep = pixels.Color(55, 55, 55);
uint32_t cDark = pixels.Color(0, 0, 0);

uint32_t channelColors[3] = {cBlue, cRed, cGreen};
uint32_t channelColorsDark[3] = {cBlueDark, cRedDark, cGreenDark};

// BUTTONS
// Encoder Button, Channel Button, Mode Button, Shift Button, Clock Button
const int buttonsLength = 5;
Button buttons[buttonsLength] = {Button(24), Button(28), Button(29), Button(30), Button(31)};
int buttonValues[buttonsLength] = {0, 0, 0, 0, 0};
int lastButtonValues[buttonsLength] = {0, 0, 0, 0, 0};

int encoderButtonIndex = 0;
int channelButtonIndex = 1;
int modeButtonIndex = 2;
int shiftButtonIndex = 3;
int clockButtonIndex = 4;

// ENCODER
Encoder encoder = Encoder(26, 27);
int encoderValue = 0;
long lastEncoderChangeTime = 0;

// SEQUENCERS
const int channelsLength = 3;
int activeChannel = 0;

bool editModeActive = 0;
int activeEditStep = 0;

Sequencer sequencers[channelsLength] = {Sequencer(), Sequencer(), Sequencer()};
int outputStates[channelsLength] = {0, 0, 0};

unsigned long editModeThresholdTime = 2000;

// CLOCK
int tempo = 128;

long bpmToTimerInterval(int bpm)
{
  // 60 seconds / beats per minute * 1000000 microseconds / 4 beats / (4 ratchets * 2 gate states)
  float microseconds = 31250.0 * (60.0 / bpm);
  return 1L * microseconds;
}

void setTempo()
{
  long clockInterval = bpmToTimerInterval(tempo);
  Timer1.setPeriod(clockInterval);
}

void incTempo()
{
  if (tempo > 40)
  {
    tempo = tempo - 1;
    setTempo();
  }
}

void decTempo()
{
  if (tempo < 512)
  {
    tempo = tempo + 1;
    setTempo();
  }
}

void setup()
{
  long clockInterval = bpmToTimerInterval(tempo);

  Timer1.initialize(clockInterval);
  Timer1.attachInterrupt(clock);

  pixels.begin();
  pixels.setBrightness(20);

  pinMode(32, OUTPUT);

  lastEncoderChangeTime = millis();
}

void switchChannel()
{
  activeChannel = (activeChannel + 1) % channelsLength;
}

void readButtonValues()
{
  for (int i = 0; i < buttonsLength; i++)
  {
    buttonValues[i] = buttons[i].read();
  }
}

void handleButtonStates()
{
  if (buttonValues[channelButtonIndex] == 1 && lastButtonValues[channelButtonIndex] == 0)
  {
    switchChannel();
  }
}

void cacheButtonValues()
{
  for (int i = 0; i < buttonsLength; i++)
  {
    lastButtonValues[i] = buttonValues[i];
  }
}

void readEncoderValue()
{
  encoderValue = encoder.read();
}

void handleEncoder()
{
  int encoderButtonValue = buttonValues[encoderButtonIndex];
  int lastEncoderButtonValue = lastButtonValues[encoderButtonIndex];

  if (editModeActive == 1 && encoderButtonValue == 1 && lastEncoderButtonValue == 0)
  {
    sequencers[activeChannel].toggleStep(activeEditStep);
  }
  else if (encoderValue > 0)
  {
    // Clock
    if (buttonValues[clockButtonIndex] == 1)
    {
      incTempo();
    }
    // Pattern offset
    else if (buttonValues[shiftButtonIndex] == 1)
    {
      sequencers[activeChannel].incOffset();
    }
    // Pattern length
    else if (buttonValues[encoderButtonIndex] == 1)
    {
      sequencers[activeChannel].incLength();
    }
    else
    {
      int patternLength = sequencers[activeChannel].getLength();

      if (editModeActive == 1)
      {
        activeEditStep = (activeEditStep + 1) % patternLength;
      }

      editModeActive = 1;
      lastEncoderChangeTime = millis();
    }
  }
  else if (encoderValue < 0)
  {
    // Clock
    if (buttonValues[clockButtonIndex] == 1)
    {
      decTempo();
    }
    // Pattern offset
    else if (buttonValues[shiftButtonIndex] == 1)
    {
      sequencers[activeChannel].decOffset();
    }
    // Pattern length
    else if (buttonValues[encoderButtonIndex] == 1)
    {
      sequencers[activeChannel].decLength();
    }
    // Pattern steps
    else
    {
      int patternLength = sequencers[activeChannel].getLength();

      if (editModeActive == 1)
      {
        activeEditStep = (patternLength + activeEditStep - 1) % patternLength;
      }

      editModeActive = 1;
      lastEncoderChangeTime = millis();
    }
  }
}

void handleSequencerPatterns()
{
  for (int channel = 0; channel < channelsLength; channel++)
  {
    int length = sequencers[channel].getLength();
    int timer = sequencers[channel].getTimer();
    int *pattern = sequencers[channel].getPattern();

    // Handle pattern audio trigger
    if (channel == 0)
    {
      int currentStepIndex = timer / 8;

      bool isStepActive = pattern[currentStepIndex] == 1;
      bool isGateHigh = timer % 8 < 4;

      if (isStepActive && isGateHigh)
      {
        digitalWrite(32, HIGH);
      }
      else
      {
        digitalWrite(32, LOW);
      }
    }

    // Handle pattern LEDs
    if (channel == activeChannel)
    {
      uint32_t channelColor = channelColors[channel];
      uint32_t channelColorDark = channelColorsDark[channel];

      // Draw pattern on LED Ring
      pixels.clear();
      for (int i = 0; i < length; i++)
      {
        if (editModeActive == 1 && i == activeEditStep && pattern[i] == 1)
        {
          pixels.setPixelColor(15 - i, cActiveStep);
        }
        else if (editModeActive == 1 && i == activeEditStep)
        {
          pixels.setPixelColor(15 - i, cInactiveStep);
        }
        // If step at index in pattern is active
        else if (pattern[i] == 1)
        {
          // If timer value equals step index
          if (i == timer / 8 && editModeActive == 0) // Step index = timer value * 16 (ratchets) * 2 (Gate on/off)
            pixels.setPixelColor(15 - i, cActiveStep);
          else
            pixels.setPixelColor(15 - i, channelColor);
        }
        else
        {
          if (i == timer / 8 && editModeActive == 0) // Step index = timer value * 16 (ratchets) * 2 (Gate on/off)
            pixels.setPixelColor(15 - i, cInactiveStep);
          else
            pixels.setPixelColor(15 - i, channelColorDark);
        }
      }
      pixels.show();
    }
  }
}

void loop()
{
  if (editModeActive == 1 && millis() - lastEncoderChangeTime > editModeThresholdTime)
  {
    editModeActive = 0;
    activeEditStep = 0;
  }

  readButtonValues();
  readEncoderValue();

  handleButtonStates();
  handleEncoder();
  handleSequencerPatterns();

  cacheButtonValues();
}

void clock()
{
  for (int i = 0; i < channelsLength; i++)
  {
    sequencers[i].incTimer();
  }
}