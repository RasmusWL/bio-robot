#define DANGERSENSE_TIME 5 * 1000 // in milliseconds
#define DANGERSENSE_STAY_PUT_TIME 2 * 1000

#define DANGERSENSE_NONE 0
#define DANGERSENSE_LEFT 90 //150
#define DANGERSENSE_LEFT_MIDDLE 120 //165
#define DANGERSENSE_MIDDLE 150// 180
#define DANGERSENSE_RIGHT_MIDDLE -120 //-165
#define DANGERSENSE_RIGHT -90 //-150

#define DANGER_DIST_PANIC 15
#define DANGER_DIST_INTIMIDATE 23
#define DANGER_DIST_STAY_PUT 30

#define DANGER_INTIMIDATE_UPDATESPEED 100

long _lastIntimidation;
char _intimidateCurrentCol;

void dangersense_intimidateUpdate()
{
    if ( millis() - _lastIntimidation > DANGER_INTIMIDATE_UPDATESPEED  )
    {
        if ( _intimidateCurrentCol == 0 )
        {
            led_setLED(255,0,0);
        }
        else if ( _intimidateCurrentCol == 1 )
        {
            led_setLED(255,255,0);
        }
        else if ( _intimidateCurrentCol == 2 )
        {
            led_setLED(0,0,255);
        }
        else if ( _intimidateCurrentCol == 3 )
        {
            led_setLED(255,0,255);
        }
        else if ( _intimidateCurrentCol == 4 )
        {
            led_setLED(0,255,0);
        }
        else if ( _intimidateCurrentCol == 5 )
        {
            led_setLED(0,255,255);
        }

        _intimidateCurrentCol++;

        if (_intimidateCurrentCol == 6)
        {
            _intimidateCurrentCol = 0;
        }
    }
}

prox_t dangersense_readImpresise()
{
    return prox_all_averaged(30,1);
}

prox_t dangersense_readPrecise()
{
    return prox_all_averaged(150,1);
}

int dangersense_dangersense(int color)
{
    unsigned long endTime = millis() + DANGERSENSE_TIME;

    prox_t reading;

    int leftMin = 100;
    int leftMax = 0;

    int middleMin = 100;
    int middleMax = 0;

    int rightMin = 100;
    int rightMax = 0;

    for ( int i = 0; i < 20; i++)
    {
        prox_t reading = dangersense_readImpresise();

        leftMin = min(leftMin, reading.left);
        middleMin = min(middleMin, reading.middle);
        rightMin = min(rightMin, reading.right);

        leftMax = max(leftMax, reading.left);
        middleMax = max(middleMax, reading.middle);
        rightMax = max(rightMax, reading.right);
    }

    // leftMin = leftMin - (leftMin/10);
    // middleMin = middleMin - (middleMin/10);
    // rightMin = rightMin - (rightMin/10);

    // leftMax = leftMax + (leftMax/10);
    // middleMax = middleMax + (middleMax/10);
    // rightMax = rightMax + (rightMax/10);

    Serial.println("dangersense: min/max vals");

    print3Digit(leftMin);
    print3Digit(middleMin);
    print3Digit(rightMin);
    Serial.print("\n");

    print3Digit(leftMax);
    print3Digit(middleMax);
    print3Digit(rightMax);
    Serial.print("\n");

    int result = DANGERSENSE_NONE;

    _lastIntimidation = 0;
    _intimidateCurrentCol = 0;

    while ( millis() < endTime )
    {
        reading = dangersense_readPrecise();

        if ( reading.left < leftMin && reading.left < DANGER_DIST_PANIC )
        {
            Serial.println("left");
            result = DANGERSENSE_LEFT;
        }

        if ( reading.right < rightMin && reading.right < DANGER_DIST_PANIC)
        {
            Serial.println("right");
            if (result == DANGERSENSE_LEFT) { result = DANGERSENSE_MIDDLE; }
            else { result = DANGERSENSE_RIGHT; }
        }

        if ( reading.middle < middleMin && reading.middle < DANGER_DIST_PANIC)
        {
            Serial.println("middle");
            if      ( result == DANGERSENSE_LEFT ) { return DANGERSENSE_LEFT_MIDDLE; }
            else if ( result == DANGERSENSE_RIGHT ) { return DANGERSENSE_RIGHT_MIDDLE; }

            result = DANGERSENSE_MIDDLE;
        }

        if ( result == DANGERSENSE_MIDDLE && reading.right < reading.left )
        {
            result = -DANGERSENSE_MIDDLE;
        }

        if (result != DANGERSENSE_NONE)
        {
            led_off();
            return result;
        }
        else if ( (reading.left < leftMin     && reading.left < DANGER_DIST_INTIMIDATE)
               || (reading.middle < middleMin && reading.middle < DANGER_DIST_INTIMIDATE)
               || (reading.right < rightMin   && reading.right < DANGER_DIST_INTIMIDATE)
            )
        {
            endTime = max( endTime , millis() + DANGERSENSE_STAY_PUT_TIME );
            dangersense_intimidateUpdate();
            Serial.println("go away you creep!");
        }
        else if ( (reading.left < leftMin     && reading.left < DANGER_DIST_STAY_PUT)
               || (reading.middle < middleMin && reading.middle < DANGER_DIST_STAY_PUT)
               || (reading.right < rightMin   && reading.right < DANGER_DIST_STAY_PUT)
            )
        {
            endTime = max( endTime , millis() + DANGERSENSE_STAY_PUT_TIME );
            Serial.println("uh, somethings there!");
            led_showDim(color);
        }
        else
        {
            led_showColour(color);
        }
    }

    return DANGERSENSE_NONE;
}

bool dangersense_shouldPanic(int data)
{
    return data != DANGERSENSE_NONE;
}
