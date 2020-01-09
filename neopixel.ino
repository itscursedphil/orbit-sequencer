#include <Adafruit_NeoPixel.h>
#include "src/Encoder/Encoder.h"
#include "src/Button/Button.h"
#include "src/Sequencer/Sequencer.h"

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(16, 22, NEO_GRB + NEO_KHZ800);

uint32_t cBlue = pixels.Color(0, 0, 255);
uint32_t cBlueDark = pixels.Color(0, 0, 20);
uint32_t cRed = pixels.Color(255, 0, 0);
uint32_t cRedDark = pixels.Color(20, 0, 0);
uint32_t cGreen = pixels.Color(0, 255, 0);
uint32_t cGreenDark = pixels.Color(0, 20, 0);
uint32_t cDark = pixels.Color(0, 0, 0);

uint32_t channelColors[3] = {cBlue, cRed, cGreen};
uint32_t channelColorsDark[3] = {cBlueDark, cRedDark, cGreenDark};

// Encoder Button, Channel Button, Mode Button
const int buttonsLength = 4;
Button buttons[buttonsLength] = {Button(24), Button(28), Button(29), Button(30)};
int buttonValues[buttonsLength] = {0, 0, 0, 0};
int lastButtonValues[buttonsLength] = {0, 0, 0, 0};

int encoderButtonIndex = 0;
int channelButtonIndex = 1;
int modeButtonIndex = 2;
int shiftButtonIndex = 3;

Encoder encoder = Encoder(26, 27);
int count = 0;

const int channelsLength = 3;
int channel = 0;

Sequencer sequencers[channelsLength] = {Sequencer(), Sequencer(), Sequencer()};

void switchChannel()
{
  channel = (channel + 1) % channelsLength;
}

void setup()
{
  pixels.begin();
  pixels.setBrightness(20);

  Serial.begin(9600);
}

void loop()
{
  // Read button values
  for (int i = 0; i < buttonsLength; i++)
  {
    buttonValues[i] = buttons[i].read();
  }

  if (buttonValues[channelButtonIndex] == 1 && lastButtonValues[channelButtonIndex] == 0)
  {
    switchChannel();
  }

  int encoderValue = encoder.read();

  if (encoderValue > 0)
  {
    if (buttonValues[shiftButtonIndex] == 1)
    {
      sequencers[channel].incOffset();
    }
    else if (buttonValues[encoderButtonIndex] == 1)
    {
      sequencers[channel].incLength();
    }
    else
    {
      sequencers[channel].incSteps();
    }
  }
  else if (encoderValue < 0)
  {
    if (buttonValues[shiftButtonIndex] == 1)
    {
      sequencers[channel].decOffset();
    }
    else if (buttonValues[encoderButtonIndex] == 1)
    {
      sequencers[channel].decLength();
    }
    else
    {
      sequencers[channel].decSteps();
    }
  }

  int length = sequencers[channel].getLength();
  int *pattern = sequencers[channel].getPattern();

  uint32_t channelColor = channelColors[channel];
  uint32_t channelColorDark = channelColorsDark[channel];

  pixels.clear();
  for (int i = 0; i < length; i++)
  {
    if (pattern[i] == 1)
    {
      pixels.setPixelColor(15 - i, channelColor);
    }
    else
    {
      pixels.setPixelColor(15 - i, channelColorDark);
    }
  }
  pixels.show();

  for (int i = 0; i < buttonsLength; i++)
  {
    lastButtonValues[i] = buttonValues[i];
  }
}
