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

    led_setup();
    motor_setup();
    encoder_setup();
    colorsens_setup();

    for(int i = 0; i < HISTORY_LENGTH; i++)
    {
        actionHistory[i] = -1;
    }
}

void loop()
{
    if (Serial.available() == 0) { delay(100); return; }

    char cmd = Serial.read();

    if ( cmd == 80 ) // P
    {
        int prox0CM = measureProxSensor(PROX_0_PIN);
        int prox1CM = measureProxSensor(PROX_1_PIN);
        int prox2CM = measureProxSensor(PROX_2_PIN);

        Serial.println("Proxymity sensors debug");
        print3Digit(prox0CM);
        print3Digit(prox1CM);
        print3Digit(prox2CM);
        Serial.print("\n");
    }
    else if ( cmd == 'C' )
    {
        char cmd = Serial.read();

        if ( cmd == 'C' )
        {
            colorsens_calibrate();
        }
        else if (cmd == 'M' )
        {
            Serial.println("Color sensors debug");
            colorsens_debug();
        }
    }
    else if ( cmd == 'M' )
    {
        int leftSpeed = Serial.parseInt();
        Serial.read(); // the comma
        int rightSpeed = Serial.parseInt();

        motor_setLeftSpeed(leftSpeed);
        motor_setRightSpeed(rightSpeed);
    }
    else if ( cmd == 'D' )
    {
        led_disco();
    }
    else if ( cmd == 'L' )
    {
        char cmd = Serial.read();

        if      ( cmd == 'R' ) { led_setLED(255,0,0); }
        else if ( cmd == 'G' ) { led_setLED(0,255,0); }
        else if ( cmd == 'B' ) { led_setLED(0,0,255); }
        else if ( cmd == 'D' ) { led_setLED(0,0,0); }
        else if ( cmd == 'W' ) { led_setLED(255,255,255); }
    }
}


void print3Digit(int num)
{
    if (num < 10)
    {
        Serial.print("  ");
    }
    else if (num < 100)
    {
        Serial.print(" ");
    }

    Serial.print(num);
}
