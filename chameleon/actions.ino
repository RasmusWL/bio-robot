#define DEBUG_ACTIONS true

void action_execute(action_t action)
{
    bool isNewAction = action.id != action_history[0].id;

    if ( isNewAction )
    {
        encoder_reset();
    }

    // ------------------------------------------------------------ //

    if ( action.type == ACTION_STRAIGHT )
    {
        int leftSpeed = 70;
        int rightSpeed = 62;

        if ( leftCount < 250 || rightCount < 250 )
        {
            leftSpeed = leftSpeed - (leftSpeed/50);
            rightSpeed = rightSpeed - (rightSpeed/50);
        }

        motor_setLeftSpeed(leftSpeed);
        motor_setRightSpeed(rightSpeed);

        delay(40);

        if (DEBUG_ACTIONS) Serial.println("A: forwards");
    }
    else if ( action.type == ACTION_TURN )
    {
        int leftSpeed = 0;
        int rightSpeed = 0;

        if ( !isNewAction )
        {
            if ( leftCount + 5 < rightCount )
            {
                leftSpeed = 2;
            }
            else if ( leftCount > rightCount + 5 )
            {
                rightSpeed = 2;
            }
        }

        if ( action.param < 0 )
        {
            leftSpeed = -62 - leftSpeed;
            rightSpeed = 60 + rightSpeed;
        }
        else
        {
            leftSpeed = 60 + leftSpeed;
            rightSpeed = -62 - rightSpeed;
        }

        motor_setSpeeds(leftSpeed, rightSpeed);

        delay(20);

        if (DEBUG_ACTIONS) Serial.println("A: turn");
    }
    else if ( action.type == ACTION_STOP )
    {
        motor_stop();
        if (DEBUG_ACTIONS) Serial.println("A: stop");
    }
    else if ( action.type == ACTION_ONE_WHEEL )
    {
        if ( action.param == PARAM_ONE_WHEEL_LEFT )
        {
            motor_setSpeeds(60,0);
        }
        else
        {
            motor_setSpeeds(0,60);
        }

        delay(40);

        if (DEBUG_ACTIONS) Serial.println("A: one wheel");
    }

    if ( action.id != action_history[0].id )
    {
        for (int i = HISTORY_LENGTH-2; i >= 0; i--)
        {
            action_history[i+1].type  = action_history[i].type;
            action_history[i+1].param = action_history[i].param;
            action_history[i+1].id = action_history[i].id;
        }

        action_history[0].type  = action.type;
        action_history[0].param = action.param;
        action_history[0].id    = action.id;
    }
}

unsigned int last_id = 0;

int action_new_id()
{
    if ( last_id < 0 ) { return 1; }

    return (last_id++);
}

int action_degTurned( int turnWay )
{
    // t1168
    // T1250

    long ticks = (leftCount + rightCount)/2;

    if ( turnWay < 0 )
    {
        return ( (ticks * 360) / 1250 );
        // T1250
    }
    else
    {
        return ( (ticks * 360) / 1150 );
        // t1150
    }
}
