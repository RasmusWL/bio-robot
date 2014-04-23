#include <ADJDS311.h>
#include <Wire.h>
#include <avr/pgmspace.h>
#include "pins.h"
#include "colordata.h"
#include "actions.h"

#define DEBUG false

volatile int leftCount;
volatile int rightCount;

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

    Serial.println("Ready");
}

void err()
{
    while (Serial.available() != 0)
    {
        Serial.read();
    }
    Serial.println("err");
}

void loop()
{
    if (Serial.available() == 0) { delay(100); return; }

    char cmd = Serial.read();

    if ( cmd == 'P' || cmd == 'p' )
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
    else if ( cmd == 'C' || cmd == 'c' )
    {
        char cmd = Serial.read();

        if ( cmd == 'C' || cmd == 'c' )
        {
            colorsens_calibrate();
        }
        else if (cmd == 'M' || cmd == 'm' )
        {
            colorsens_debug();
        }
        else if (cmd == 'A' || cmd == 'a' )
        {
            colorsens_activate_showoff();
        }
        else { err(); return; }
    }
    else if ( cmd == 'M' || cmd == 'm' )
    {
        int leftSpeed = Serial.parseInt();
        Serial.read(); // the comma
        int rightSpeed = Serial.parseInt();

        motor_setLeftSpeed(leftSpeed);
        motor_setRightSpeed(rightSpeed);
    }
    else if ( cmd == 'D' || cmd == 'd' )
    {
        led_disco();
    }
    else if ( cmd == 'L' || cmd == 'l' )
    {
        char cmd = Serial.read();

        if      ( cmd == 'R' || cmd == 'r' ) { led_setLED(255,0,0); }
        else if ( cmd == 'G' || cmd == 'g' ) { led_setLED(0,255,0); }
        else if ( cmd == 'B' || cmd == 'b' ) { led_setLED(0,0,255); }
        else if ( cmd == 'D' || cmd == 'd' ) { led_setLED(0,0,0); }
        else if ( cmd == 'W' || cmd == 'w' ) { led_setLED(255,255,255); }
        else if ( cmd == '0' ) { led_showColour(0); }
        else if ( cmd == '1' ) { led_showColour(1); }
        else if ( cmd == '2' ) { led_showColour(2); }
        else if ( cmd == '3' ) { led_showColour(3); }
        else if ( cmd == '4' ) { led_showColour(4); }
        else if ( cmd == '5' ) { led_showColour(5); }
        else { err(); return; }
    }
    else if (cmd == 'E' || cmd == 'e' )
    {
        int oldLeft = leftCount;
        int oldRight = rightCount;

        delay(1000);

        int deltaLeft = leftCount - oldLeft;
        int deltaRight = rightCount - oldRight;

        Serial.print(deltaLeft);
        Serial.print(" | ");
        Serial.print(deltaRight);
        Serial.print("\n");
    }
    else { err(); return; }

    Serial.println("done");
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
