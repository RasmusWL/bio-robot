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
    Serial.println("Initializing");

    // ------------------------------------------------------------ //

    led_setup();
    motor_setup();
    encoder_setup();
    colorsens_setup();

    for(int i = 0; i < HISTORY_LENGTH; i++)
    {
        action_history[i].id = 0;
    }

    Serial.println("Ready");
}

void loop()
{
    if ( DEBUG ) { debug_loop(); }
    else         { test_loop(); }
}

robot_info_t robot_info;

void real_loop()
{
    info_t info;

    info.prox0CM = measureProxSensor(PROX_0_PIN);
    info.prox1CM = measureProxSensor(PROX_1_PIN);
    info.prox2CM = measureProxSensor(PROX_2_PIN);
    info.color0Match = colorsens_measure(0);
    info.color1Match = colorsens_measure(1);
    info.color2Match = colorsens_measure(2);

    action_t action;
    action.type = ACTION_STRAIGHT;
    action.param = 10000;

    // ------------------------------------------------------------ //

    locatecolor_action(&info, &action);
    avoidance_action(&info, &action);

    action_execute(&action);
}

#define TURN_CIRCUMFERENCE 36.13

void turnForMe(int leftSpeed, int rightSpeed, int turnTicks)
{
    encoder_reset();

    motor_setLeftSpeed(leftSpeed);
    motor_setRightSpeed(rightSpeed);

    while ( rightCount < turnTicks || leftCount < turnTicks )
    {

    }

    motor_stop();
}


int proxHit = 0;
int lastColorHiddenOn = -1;
int leftSpeed = 90;
int rightSpeed = 100;

int lastLeftGoal = 0;
int lastRightGoal = 0;

bool forwards = false;

void test_loop()
{
    int prox1CM = measureProxSensor(PROX_1_PIN);
    int prox2CM = measureProxSensor(PROX_2_PIN);

    int color1Match = -1;//colorsens_measure(1);

    if (color1Match != -1)
    {
        Serial.print("matched color");
        Serial.println(color1Match);

        if ( color1Match != lastColorHiddenOn )
        {
            Serial.println("showing it");

            motor_stop();
            lastColorHiddenOn = color1Match;
            led_showColour(color1Match);
            delay(2000);
            return;
        }
    }
    else if (lastColorHiddenOn != -1)
    {
        Serial.println("turning it off");
        led_setLED(0,0,0);
        lastColorHiddenOn = -1;
    }

    if (prox1CM <= 23 || prox2CM <= 23 )
    {
        proxHit ++;
    }

    if (proxHit >= 2)
    {
        Serial.println("need to turn");
        if ( forwards )
        {
            motor_stop();
            forwards = false;
        }

        int ticks = random(50, 100);
        bool turnCCW = true;
        if ( abs(prox1CM - prox2CM) > 10 )
        {
            turnCCW = prox1CM < prox2CM;
        }
        else
        {
            turnCCW = random(0, 1) == 0;
        }

        if (turnCCW) { turnForMe(50,-50,ticks); }
        else         { turnForMe(-50,50,ticks); }

        proxHit = 0;
    }
    else
    {
        Serial.println("forwards!");
        forwards = true;

        encoder_reset();
        delay(50);

        int leftGoal = 40;
        int rightGoal = 40;
        int change = 2;

        if ( prox1CM <= 35 )
        {
            if ( lastRightGoal != 20 )
            {
                rightSpeed = rightSpeed / 2;
            }
            rightGoal = 20;
        }
        else if ( prox2CM <= 35 )
        {
            if ( lastLeftGoal != 20 )
            {
                leftSpeed = leftSpeed / 2;
            }
            leftGoal = 20;
        }

        if ( leftCount < leftGoal - change )
        {
            leftSpeed += change;
        }
        else if (leftCount > leftGoal + change )
        {
            leftSpeed -= change;
        }
        motor_setLeftSpeed(leftSpeed);

        if ( rightCount < rightGoal - change )
        {
            rightSpeed += change;
        }
        else if (rightCount > rightGoal + change )
        {
            rightSpeed -= change;
        }
        motor_setRightSpeed(rightSpeed);

        lastLeftGoal = leftGoal;
        lastRightGoal = rightGoal;
    }
}

void debug_loop()
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
    else if ( cmd == 'V' || cmd == 'v' )
    {
        Serial.print("0= ");
        colorsens_printCalibration(0);
        Serial.print("1= ");
        colorsens_printCalibration(1);
        Serial.print("2= ");
        colorsens_printCalibration(2);
    }
    else if ( cmd == 'X' || cmd == 'x' )
    {
        int num = Serial.parseInt();
        Serial.read();
        int colorGain = Serial.parseInt();
        Serial.read();
        int clearGain = Serial.parseInt();

        colorsens_setCalibration(num,colorGain,clearGain);
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
        else if (cmd == 'S' || cmd == 's' )
        {
            int num = Serial.parseInt();
            colorsens_activate_special(num);
        }
        else if (cmd == 'A' || cmd == 'a' )
        {
            colorsens_activate_showoff();
        }
        else if (cmd == '0' )
        {
            colorsens_debug_sens(0);
        }
        else if (cmd == '1' )
        {
            colorsens_debug_sens(1);
        }
        else if (cmd == '2' )
        {
            colorsens_debug_sens(2);
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
    else if (cmd == 'T' || cmd == 't' )
    {
        int ticks = Serial.parseInt();



        encoder_reset();

        if ( cmd == 'T' )
        {
            motor_setLeftSpeed(-50);
            motor_setRightSpeed(50);
        }
        else
        {
            motor_setLeftSpeed(50);
            motor_setRightSpeed(-50);
        }

        while ( rightCount < ticks || leftCount < ticks )
        {

        }
        motor_setLeftSpeed(0);
        motor_setRightSpeed(0);

        Serial.print("l=");
        Serial.print(leftCount);
        Serial.print(" r=");
        Serial.print(rightCount);
        Serial.print("\n");
    }

    // backwards :)
    // m-30,-50
    else { err(); return; }

    Serial.println("done");
}

void err()
{
    while (Serial.available() != 0)
    {
        Serial.read();
    }
    Serial.println("err");
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
