#include <Adafruit_NeoPixel.h>
#include "src/Encoder/Encoder.h"
#include "src/Button/Button.h"
#include "src/Sequencer/Sequencer.h"

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(16, 22, NEO_GRB + NEO_KHZ800);

Encoder encoder = Encoder(26, 27);
int count = 0;

Button button = Button(24);
int lastButtonValue = 0;

Sequencer seq = Sequencer();

void setup()
{
  pixels.begin();
  pixels.setBrightness(20);

  Serial.begin(9600);
}

uint32_t c_blue = pixels.Color(0, 0, 255);
uint32_t c_blue_dark = pixels.Color(0, 0, 20);
uint32_t c_dark = pixels.Color(0, 0, 0);

void loop()
{
  // int buttonValue = button.read();

  // if (buttonValue != lastButtonValue && buttonValue == 1)
  // {
  //   color = (color + 1) % 3;
  // }

  // lastButtonValue = buttonValue;

  int buttonValue = button.read();
  int encoderValue = encoder.read();

  if (encoderValue > 0)
  {
    if (buttonValue == 1)
    {
      seq.incLength();
    }
    else
    {
      seq.incSteps();
    }
  }
  else if (encoderValue < 0)
  {
    if (buttonValue == 1)
    {
      seq.decLength();
    }
    else
    {
      seq.decSteps();
    }
  }

  int length = seq.getLength();
  int *pattern = seq.getPattern();

  pixels.clear();
  for (int i = 0; i < length; i++)
  {
    if (pattern[i] == 1)
    {
      pixels.setPixelColor(15 - i, c_blue);
    }
    else
    {
      pixels.setPixelColor(15 - i, c_blue_dark);
    }
  }
  pixels.show();
}
