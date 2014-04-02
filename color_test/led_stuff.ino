void setLED(int red, int green, int blue)
{
  int max = 70;
  int redOut = map(red, 0,255, 0,max);
  int greenOut = map(green, 0,255, 0,max);
  int blueOut = map(blue, 0,255, 0,max);

  analogWrite(RED_PIN,   255-redOut);
  analogWrite(GREEN_PIN, 255-greenOut);
  analogWrite(BLUE_PIN,  255-blueOut);
}
