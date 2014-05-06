#include <ADJDS311.h>
#include <Wire.h>
#include <avr/pgmspace.h>

#include "pins.h"
#include "colordata.h"
#include "actions.h"
#include "colorsens.h"
#include "msg.h"
#include "proximity.h"
#include "movementdist.h"

#define DEBUG false
#define BLUETOOTH true

#define STATE_RANDOM_WALK 0
#define STATE_LOCATE_COLOR 1
#define STATE_DANGER_SENSE 2
#define STATE_PANIC 3

void loop_v3();

volatile int leftCount;
volatile int rightCount;

robot_info_t robot_info;

int state;
void setup()
{

    // ------------------------------------------------------------ //

    led_setup();
    motor_setup();
    encoder_setup();
    colorsens_setup();
    locatecolor_setup();

    for(int i = 0; i < HISTORY_LENGTH; i++)
    {
        action_history[i].id = 0;
        action_history[i].type = 0;
        action_history[i].param = 0;
    }

    state = STATE_RANDOM_WALK;

    if ( BLUETOOTH ) { bluetooth_setup(); }
    else             { Serial.begin(9600);  }

    Serial.println("Ready");
}

void loop()
{
    if ( DEBUG ) { debug_loop(); }
    else         { loop_v3(); }
}

// bluetooth info at https://learn.sparkfun.com/tutorials/using-the-bluesmirf/all
void bluetooth_setup()
{
    led_setLED(255,0,0);
    delay(1000);

    Serial.begin(115200);  // The Bluetooth Mate defaults to 115200bps
    Serial.print("$");  // Print three times individually
    Serial.print("$");
    Serial.print("$");  // Enter command mode
    delay(100);  // Short delay, wait for the Mate to send back CMD
    led_setLED(0,0,255);
    delay(500);
    Serial.println("U,9600,N");  // Temporarily Change the baudrate to 9600, no parity
    // delay(2000);
    // Serial.println("---");
    delay(200);
    // 115200 can be too fast at times for NewSoftSerial to relay the data reliably
    Serial.begin(9600);

    while ( Serial.available() != 0 )
    {
        Serial.read();
    }

    led_setLED(0,255,0);
    delay(500);
    led_off();
}

// ------------------------------------------------------------ //

void loop_v3()
{
    colormatch_t color = colorsens_measureAll();
    prox_t prox = prox_all_averaged(5, 1);

    // Serial.print("state = ");
    // Serial.println(state);

    // Serial.print("p");
    // print3Digit(prox.left);
    // print3Digit(prox.middle);
    // print3Digit(prox.right);
    // Serial.print("\n");

    // Serial.print("   ");
    // print3Digit(color.front);
    // Serial.print("   ");
    // Serial.print("\n");

    // print3Digit(color.left);
    // Serial.print("   ");
    // print3Digit(color.right);
    // Serial.print("\n");
    // Serial.print("\n");

    action_t lastAction = action_history[0];
    action_t newAction;

    if ( lastAction.type == ACTION_STRAIGHT || lastAction.type == ACTION_ONE_WHEEL )
    {
        if ( avoidance_needToStop(prox) )
        {
            newAction = {};
            newAction.id = action_new_id();
            newAction.type = ACTION_STOP;
            newAction.param = 0;

            action_execute(newAction);
            return;
        }
    }

loop_v3_state_changed:



    if ( state == STATE_RANDOM_WALK )
    {
        if ( locatecolor_willLookFor(color) )
        {
            state = STATE_LOCATE_COLOR;
            locatecolor_switchto(color);
            goto loop_v3_state_changed;
        }

        newAction = randomwalk_newAction(lastAction, prox);
    }
    else if ( state == STATE_LOCATE_COLOR )
    {
        if ( locatecolor_success(color) )
        {
            state = STATE_DANGER_SENSE;
            goto loop_v3_state_changed;
        }
        else if ( locatecolor_finished() )
        {
            led_off();

            state = STATE_RANDOM_WALK;
            randomwalk_switchto();
            goto loop_v3_state_changed;
        }

        newAction = locatecolor_newAction(lastAction, color);
    }
    else if ( state == STATE_DANGER_SENSE )
    {
        newAction.id = action_new_id();
        newAction.type = ACTION_STOP;
        newAction.param = 0;
        action_execute(newAction);

        led_showColour(color.front);
        int data = dangersense_dangersense(color.front);

        if ( dangersense_shouldPanic(data) )
        {
            // TODO : actual panic the shit away

            led_off();

            state = STATE_RANDOM_WALK;
            randomwalk_switchto();

            return;

        }
        else
        {
            led_off();

            state = STATE_RANDOM_WALK;
            randomwalk_switchto();

            return;
        }
    }

    action_execute(newAction);
}

// ------------------------------------------------------------ //

int leftSpeed;
int rightSpeed;

void findGoodSpeed()
{
    encoder_reset();
    delay(50);

    int leftGoal = 40;
    int rightGoal = 40;
    int change = 2;

    if ( leftCount < leftGoal - change )
    {
        leftSpeed += change;
    }
    else if (leftCount > leftGoal + change )
    {
        leftSpeed -= change;
    }

    if ( rightCount < rightGoal - change )
    {
        rightSpeed += change;
    }
    else if (rightCount > rightGoal + change )
    {
        rightSpeed -= change;
    }

    motor_setLeftSpeed(leftSpeed);
    motor_setRightSpeed(rightSpeed);
}

#define DEBUG_STATE_NONE 0
#define DEBUG_STATE_FIND_GOOD_SPEED 1
#define DEBUG_STATE_LOCATE_COLOR 2

int specialState = DEBUG_STATE_NONE;
unsigned long specialStateEnd;

void debug_loop()
{
    if ( specialState != DEBUG_STATE_NONE )
    {
        if ( specialState == DEBUG_STATE_FIND_GOOD_SPEED )
        {
            //findGoodSpeed();
        }
        else if (specialState == DEBUG_STATE_LOCATE_COLOR)
        {
            if ( state == STATE_LOCATE_COLOR )
            {
                loop_v3();
            }
            else
            {
                Serial.println("Not trying to find color anymore");
                specialStateEnd = 0;
            }
        }

        if ( millis() > specialStateEnd )
        {
            if ( specialState == DEBUG_STATE_FIND_GOOD_SPEED)
            {
                motor_stop();
                Serial.print(leftSpeed);
                Serial.print(" ");
                Serial.print(rightSpeed);
                Serial.print("\n");
            }

            motor_stop();
            led_off();
            specialState = DEBUG_STATE_NONE;
        }

        return;
    }

    if (Serial.available() == 0) { delay(100); return; }

    char buffer[20] = {};
    msg_t msg = msg_fromSerialParse(buffer,20);

    if ( msg.len == -1 )
    {
        err();
        return;
    }

    char cmd = msg_read(&msg);

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
    else if ( cmd == 'K' || cmd == 'k' )
    {
        colormatch_t color = colorsens_measureAll();
        state = STATE_LOCATE_COLOR;
        locatecolor_switchto(color);
        specialStateEnd = millis() + 30 * 1000;
        specialState = DEBUG_STATE_LOCATE_COLOR;
    }
    else if ( cmd == 'F' || cmd == 'f' )
    {
        //int timeInSec = msg_parseInt(&msg);
        //msg_read(&msg);
        int timeInSec = 1;

        int leftSpeed = msg_parseInt(&msg);
        msg_read(&msg);
        int rightSpeed = msg_parseInt(&msg);

        specialState = DEBUG_STATE_FIND_GOOD_SPEED;
        specialStateEnd = millis() + timeInSec * 1000;

        motor_setSpeeds(leftSpeed,rightSpeed);

        colorsens_readALot(0,2);

        // leftSpeed = 90;
        // rightSpeed = 90;
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
        int num = msg_parseInt(&msg);
        msg_read(&msg);
        int colorGain = msg_parseInt(&msg);
        msg_read(&msg);
        int clearGain = msg_parseInt(&msg);
        msg_read(&msg);

        int colorCapRed = msg_parseInt(&msg);
        msg_read(&msg);
        int colorCapGreen = msg_parseInt(&msg);
        msg_read(&msg);
        int colorCapBlue = msg_parseInt(&msg);
        msg_read(&msg);
        int colorCapClear = msg_parseInt(&msg);

        colorsens_setCalibration(num, colorGain,clearGain,
                                 colorCapRed, colorCapGreen,
                                 colorCapBlue, colorCapClear);
    }
    else if (cmd == 'S' || cmd == 's' )
    {
        int asdf = dangersense();

        if ( asdf != 0 )
        {
            panicMotherFucker(asdf);
        }
    }
    else if ( cmd == 'C' || cmd == 'c' )
    {
        char cmd = msg_read(&msg);

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
            int num = msg_parseInt(&msg);
            colorsens_activate_special(num);
        }
        else if (cmd == 'A' || cmd == 'a' )
        {
            colorsens_activate_showoff();
        }
        else if (cmd == '0' )
        {
            colorsens_debug_sens(0);
            colorsens_readALot(0,0);
        }
        else if (cmd == '1' )
        {
            colorsens_debug_sens(1);
            colorsens_readALot(1,1);
        }
        else if (cmd == '2' )
        {
            colorsens_debug_sens(2);
            colorsens_readALot(2,2);
        }
        else if (cmd == 'T' || cmd == 't')
        {
            colorsens_readALot(0,2);
        }
        else { err(); return; }
    }
    else if ( cmd == 'M' || cmd == 'm' )
    {
        int leftSpeed = msg_parseInt(&msg);
        msg_read(&msg); // the comma
        int rightSpeed = msg_parseInt(&msg);

        motor_setLeftSpeed(leftSpeed);
        motor_setRightSpeed(rightSpeed);
    }
    else if ( cmd == 'D' || cmd == 'd' )
    {
        led_disco();
    }
    else if ( cmd == 'L' || cmd == 'l' )
    {
        char cmd = msg_read(&msg);

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
    else if (cmd == 'A' || cmd == 'a' )
    {
        int red = msg_parseInt(&msg);
        msg_read(&msg);
        int green = msg_parseInt(&msg);
        msg_read(&msg);
        int blue = msg_parseInt(&msg);

        led_setLED(red,green,blue);
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
        int ticks = msg_parseInt(&msg);

        encoder_reset();

        if ( cmd == 'T' )
        {
            motor_setLeftSpeed(-90);
            motor_setRightSpeed(90);
        }
        else
        {
            motor_setLeftSpeed(90);
            motor_setRightSpeed(-90);
        }

        // bool stupid;

        // while ( leftCount < ticks )
        // {
        //     stupid = leftCount = 42;
        //     //Serial.println(leftCount);
        // }


        delay(ticks);

        motor_stop();

        Serial.print("l=");
        Serial.print(leftCount);
        Serial.print(" r=");
        Serial.print(rightCount);
        Serial.print("\n");
    }
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
