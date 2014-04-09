#include <ADJDS311.h>
#include <Wire.h>
#include <avr/pgmspace.h>
#include "pins.h"
#include "colordata.h"
#include "actions.h"

#define DEBUG false

void setup()
{
    Serial.begin(9600);

    // ------------------------------------------------------------ //

    //led_setup();
    //motor_setup();
    //encoder_setup();
    colorsens_setup();

    for(int i = 0; i < HISTORY_LENGTH; i++)
    {
        actionHistory[i] = -1;
    }
}

void loop()
{

}
