unsigned long turnStart;

void action_execute(action_t action)
{
    // action_t last_action = action_history[0];

    // if ( last_action.id == action.id )
    // {
    //     if ( action.type == ACTION_STRAIGHT )
    //     {
    //         if ( action.param < encoder_distance_traveled() )
    //         {
    //             action.type = ACTION_STOP;
    //             action.id = action_new_id();
    //         }
    //         else
    //         {
    //             return;
    //         }
    //     }
    //     else
    //     {
    //         Serial.println("Only going straight must share IDs");
    //         delay(100000);
    //         return;
    //     }
    // }

    // // ------------------------------------------------------------ //
    // // we have a "new" action we need to run

    // if ( last_action.type == ACTION_STRAIGHT )
    // {
    //     mapping_traveled( encoder_distance_traveled() );
    //     encoder_reset();
    // }

    // ------------------------------------------------------------ //

    if ( action.type == ACTION_STRAIGHT )
    {
        motor_setLeftSpeed(75);
        motor_setRightSpeed(70);

        delay(40);

        Serial.println("A: forwards");
    }
    else if ( action.type == ACTION_TURN )
    {
        if ( action.id != action_history[0].id )
        {
            turnStart = millis();
        }

        // 90 deg CW = t390
        // 90 deg CCW = T350

        if ( action.param < 0 )
        {
            motor_setLeftSpeed(-70);
            motor_setRightSpeed(70);

            // delay ( -1 * (350 * action.param) / 90 );
        }
        else
        {
            motor_setLeftSpeed(70);
            motor_setRightSpeed(-70);

            // delay ( (390 * action.param) / 90 );
        }

        // motor_stop();

        delay(20);

        // mapping_rotated(action.param);
        Serial.println("A: turn");
    }
    else if ( action.type == ACTION_STOP )
    {
        motor_stop();
        Serial.println("A: stop");
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

        Serial.println("A: one wheel");
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
    if ( turnWay < 0 )
    {
        return ( ( millis() - turnStart ) * 90 ) / 500;
    }
    else
    {
        return ( ( millis() - turnStart ) * 90 ) / 500;
    }
}
