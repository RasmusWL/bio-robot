void action_execute(action_t* action)
{
    if ( action->type == ACTION_STRAIGHT )
    {
        motor_setLeftSpeed(100);
        motor_setRightSpeed(100);
    }
    else if ( action->type == ACTION_TURN )
    {
        int time;
        int speed;

        switch (action->param) {
            case 15:
              time = 100;
              speed = 50;
              break;
            case 30:
              time = 200;
              speed = 50;
              break;
        }
    }

}
