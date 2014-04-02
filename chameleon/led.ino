#define LED_MAX 70

void led_setup()
{
    pinMode(RED_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(BLUE_PIN, OUTPUT);
}

/* Set all the LEDs to a colour
 * - values must be between 0-255 (both included)
 *
 * Our LEDs are a bit strange, so giving them full power
 *   turns them completely off. If they are provided with
 *   a low absolute value, they look strange, so we cap
 *   them with the LED_MAX value.
 */
void setLED(int red, int green, int blue)
{
  int redOut   = map(red,   0,255, 0,LED_MAX);
  int greenOut = map(green, 0,255, 0,LED_MAX);
  int blueOut  = map(blue,  0,255, 0,LED_MAX);

  analogWrite(RED_PIN,   255-redOut);
  analogWrite(GREEN_PIN, 255-greenOut);
  analogWrite(BLUE_PIN,  255-blueOut);
}
