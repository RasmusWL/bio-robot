#define PANIC_TIME 3 * 1000
#define PANIC_DRIVE_TIME 1.5 * 1000

unsigned long panic_endTime;

bool finishedTurning;
int panic_degToTurn;
int panic_turningID;

void panic_switchto(int degToTurn)
{
    panic_endTime = millis() + PANIC_TIME;
    panic_degToTurn = degToTurn;
    panic_turningID = -1;

    finishedTurning = false;
}

action_t panic_newAction(action_t lastAction, prox_t prox)
{
    action_t action = {};

    if ( ! finishedTurning )
    {
        if (panic_turningID == -1)
        {
            action.id = action_new_id();
            action.type = ACTION_TURN;
            action.param = panic_degToTurn/abs(panic_degToTurn);

            // todo : only positive degree now :)
            panic_degToTurn = abs(panic_degToTurn);

            panic_turningID = action.id;

            return action;
        }
        else if ( action_degTurned(lastAction.param) < panic_degToTurn )
        {
            action.id = lastAction.id;
            action.type = ACTION_TURN;
            action.param = lastAction.param;

            return action;
        }
        else
        {
            finishedTurning = true;

            return panic_newAction(lastAction, prox);
        }
    }
    else if ( panic_degToTurn != 0 && avoidance_shouldNotGoThisWay(prox) )
    {
        action.id = lastAction.id;
        action.type = ACTION_TURN;
        action.param = lastAction.param;

        return action;
    }
    else
    {
        if ( panic_degToTurn != 0 )
        {
            panic_degToTurn = 0;
            panic_endTime = max( panic_endTime, millis() + PANIC_DRIVE_TIME );
        }

        action.id = action_new_id();
        action.type = ACTION_STRAIGHT;
        action.param = 1000;

        return action;
    }

}

bool panic_isFinished()
{
    return finishedTurning && panic_endTime < millis();
}

bool panic_hasTurned()
{
    return finishedTurning;
}
