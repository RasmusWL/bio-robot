#define LED_RED_MAX   150
#define LED_GREEN_MAX 120
#define LED_BLUE_MAX  190

boolean _led_off;

void led_setup()
{
    pinMode(RED_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(BLUE_PIN, OUTPUT);

    pinMode(LED_ON_PIN, OUTPUT);

    _led_off = false;
    led_setLED(0,0,0);
}

void led_showColour(int color)
{
    int red   = pgm_read_byte_near(LED_VALS + color*3 + 0);
    int green = pgm_read_byte_near(LED_VALS + color*3 + 1);
    int blue  = pgm_read_byte_near(LED_VALS + color*3 + 2);

    led_setLED(red,green,blue);
}

void led_showDim(int color)
{
    int red   = pgm_read_byte_near(LED_VALS + color*3 + 0);
    int green = pgm_read_byte_near(LED_VALS + color*3 + 1);
    int blue  = pgm_read_byte_near(LED_VALS + color*3 + 2);

    led_setLED(red/2,green/2,blue/2);
}

void led_off()
{
    led_setLED(0,0,0);
}

/* Set all the LEDs to a colour
 * - values must be between 0-255 (both included)
 *
 * Our LEDs are a bit strange, so giving them full power
 *   turns them completely off. If they are provided with
 *   a low absolute value, they look strange, so we cap
 *   them with the LED_MAX value.
 */

void led_setLED(int red, int green, int blue)
{
    if ( red == 0 && green == 0 && blue == 0 )
    {
        digitalWrite(LED_ON_PIN, HIGH);
        _led_off = true;
        delay(3);
    }
    else if ( _led_off )
    {
        digitalWrite(LED_ON_PIN, LOW);
        _led_off = false;
        delay(3);
    }

    int redOut   = map(red,   0,255, 0,LED_RED_MAX);
    int greenOut = map(green, 0,255, 0,LED_GREEN_MAX);
    int blueOut  = map(blue,  0,255, 0,LED_BLUE_MAX);

    analogWrite(RED_PIN,   255-redOut);
    analogWrite(GREEN_PIN, 255-greenOut);
    analogWrite(BLUE_PIN,  255-blueOut);
}

int _red = 0;
int _green = 0;
int _blue = 0;

void led_showOff()
{
    led_setLED(0,0,0);
    _red = 0;
    _green = 0;
    _blue = 0;

    delay(100);

    for (int i = 0; i < NUM_COLORS; i++)
    {
        int red   = pgm_read_byte_near(LED_VALS + i*3 + 0);
        int green = pgm_read_byte_near(LED_VALS + i*3 + 1);
        int blue  = pgm_read_byte_near(LED_VALS + i*3 + 2);

        led_transition(red,green,blue);
        delay(100);
    }
}

void led_disco()
{
    led_setLED(0,0,0);
    _red = 0;
    _green = 0;
    _blue = 0;

    for (int i = 0; i < 5; i++)
    {
        led_transition(255,0,0);
        led_transition(255,255,0);
        led_transition(0,255,0);
        led_transition(0,255,255);
        led_transition(0,0,255);
        led_transition(255,0,255);
    }

    led_setLED(0,0,0);
}

void led_transition(int newRed, int newGreen, int newBlue)
{
    while (_red != newRed || _green != newGreen || _blue != newBlue)
    {
        if      (_red < newRed) _red++;
        else if (_red > newRed) _red--;

        if      (_green < newGreen) _green++;
        else if (_green > newGreen) _green--;

        if      (_blue < newBlue) _blue++;
        else if (_blue > newBlue) _blue--;

        led_setLED(_red,_green,_blue);
        delay(1);
    }
}
