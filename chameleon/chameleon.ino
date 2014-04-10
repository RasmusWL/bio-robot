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
    else if ( cmd == 67 ) // C
    {
        char cmd = Serial.read();

        if ( cmd == 67 ) // C
        {
            colorsens_calibrate();
        }
        else if (cmd == 77 ) // M
        {
            Serial.println("Color sensors debug");
            colorsens_debug();
        }
    }
    else if ( cmd == 77 ) // M
    {
        int leftSpeed = Serial.parseInt();
        Serial.read(); // the comma
        int rightSpeed = Serial.parseInt();

        motor_setLeftSpeed(leftSpeed);
        motor_setRightSpeed(rightSpeed);
    }
    else if ( cmd == 68 ) // D
    {
        led_disco();
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
