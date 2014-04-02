#include <Wire.h>

#define RED_PIN 3
#define GREEN_PIN 6
#define BLUE_PIN 5

void setup()
{

}

void loop()
{
  int delayTime = 500;
  setLED(255,0,0);
  delay(delayTime);
  setLED(0,255,0);
  delay(delayTime);
  setLED(0,0,255);
  delay(delayTime);
}

void setLED(int red, int green, int blue)
{
  int max = 150;
  int redOut = map(red, 0,255, 0,max);
  int greenOut = map(green, 0,255, 0,max);
  int blueOut = map(blue, 0,255, 0,max);

  analogWrite(RED_PIN,   255-redOut);
  analogWrite(GREEN_PIN, 255-greenOut);
  analogWrite(BLUE_PIN,  255-blueOut);
}
