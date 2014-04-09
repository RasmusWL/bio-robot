#include <ADJDS311.h>
#include <Wire.h>
#include <avr/pgmspace.h>
#include "pins.h"
#include "colordata.h"

#define DEBUG false

void setup()
{
    Serial.begin(9600);

    // ------------------------------------------------------------ //

    //led_setup();
    //motor_setup();
    //encoder_setup();
    colorsens_setup();
}

void loop()
{

}
