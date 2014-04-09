#include <Wire.h>

#define RED_PIN 5
#define GREEN_PIN 6
#define BLUE_PIN 9

void setup()
{
    pinMode(RED_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(BLUE_PIN, OUTPUT);
}

void loop()
{
  int delayTime = 800;
  setLED(255,0,0);
  delay(delayTime);
  setLED(0,255,0);
  delay(delayTime);
  setLED(0,0,255);
  delay(delayTime);
}

void setLED(int red, int green, int blue)
{
  int max = 100;
  int redOut = map(red, 0,255, 0,100);
  int greenOut = map(green, 0,255, 0,75);
  int blueOut = map(blue, 0,255, 0,130);

  analogWrite(RED_PIN,   255-redOut);
  analogWrite(GREEN_PIN, 255-greenOut);
  analogWrite(BLUE_PIN,  255-blueOut);
}
