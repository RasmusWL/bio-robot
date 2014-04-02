#include <ADJDS311.h>
#include <Wire.h>
#include <avr/pgmspace.h>
#include "pins.h"

void setup()
{
    Serial.begin(9600);
    Wire.begin();

    // ------------------------------------------------------------ //

    led_setup();
    motor_setup();
    encoder_setup();
    colorsens_setup();
}

void loop()
{

}
