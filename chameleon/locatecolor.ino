#define MAX_TIME_TO_LOCATE 5 * 1000
#define TIME_AFTER_LAST_COLOR 3 * 1000

unsigned long locatecolor_endTime;
int _lastColorLookedFor;

void locatecolor_finishedHiding()
{
    locatecolor_endTime = millis();
}

void locatecolor_setup()
{
    _lastColorLookedFor = -2;
    locatecolor_endTime = 0;
}

bool locatecolor_willLookFor(colormatch_t color)
{
    return _locatecolor_getNewColor(color) != -1;
}

void locatecolor_switchto( colormatch_t color )
{
    _lastColorLookedFor = _locatecolor_getNewColor(color);
    led_showDim(_lastColorLookedFor);
    locatecolor_endTime = millis() + MAX_TIME_TO_LOCATE;
}

int _locatecolor_getNewColor( colormatch_t color )
{
    bool canLookForLastAgain = ( locatecolor_endTime == 0 || locatecolor_endTime + TIME_AFTER_LAST_COLOR < millis() );

    if ( color.front != -1
        && (color.front != _lastColorLookedFor || canLookForLastAgain )
        )
    {
        return color.front;
    }
    else if ( color.left != -1
            && ( color.left != _lastColorLookedFor || canLookForLastAgain )
            )
    {
        return color.left;
    }
    else if ( color.right != -1
            && ( color.right != _lastColorLookedFor || canLookForLastAgain )
            )
    {
        return color.right;
    }

    return -1;
}

bool locatecolor_isFinished()
{
    return millis() > locatecolor_endTime;
}

bool locatecolor_success(colormatch_t color)
{
    return color.front == color.left && color.front == color.right && color.front != -1;
}

int _locatecolor_turnId;

action_t giveUp()
{
    action_t action = {};
    action.id = action_new_id();
    action.type = ACTION_STOP;
    locatecolor_endTime = millis();
    return action;
}

action_t locatecolor_newAction(action_t lastAction, colormatch_t color, prox_t prox)
{
    action_t action = {};
    action.type = ACTION_STRAIGHT;
    action.param = 1000;
    action.id = action_new_id();

    if ( lastAction.type == ACTION_STOP && avoidance_needToStop(prox) )
    {
        if ( color.left != _lastColorLookedFor && color.right != _lastColorLookedFor )
        {
            return giveUp();
        }

        // a bit hacky, but will make the robot spin untill front is on good colour
        color.front = -1;
    }

    if ( lastAction.type == ACTION_TURN && lastAction.id == _locatecolor_turnId )
    {
        if ( color.front != _lastColorLookedFor && action_degTurned(lastAction.param) < 360 )
        {
            action.type = ACTION_TURN;
            action.param = lastAction.param;
            action.id = lastAction.id;
            return action;
        }
        else
        {
            action.type = ACTION_STOP;
            action.param = 0;
            return action;
        }
    }

    if ( color.front == color.left && color.front == color.right && color.front == _lastColorLookedFor )
    {
        action.type = ACTION_STOP;
        action.param = 0;
        return action;
    }

    if ( color.front != _lastColorLookedFor)
    {
        if ( color.left == _lastColorLookedFor && color.right == _lastColorLookedFor )
        {
            action.type = ACTION_TURN;
            action.param = TURN_WAY_LEFT;
        }
        else if ( color.left == _lastColorLookedFor )
        {
            action.type = ACTION_TURN;
            action.param = TURN_WAY_LEFT;
        }
        else if ( color.right == _lastColorLookedFor )
        {
            action.type = ACTION_TURN;
            action.param = TURN_WAY_RIGHT;
        }
    }
    else
    {
        if ( color.left != _lastColorLookedFor && color.right != _lastColorLookedFor )
        {
            action.type = ACTION_STRAIGHT;
            action.param = 1000;
        }
        else if ( color.left == _lastColorLookedFor )
        {
            action.type = ACTION_ONE_WHEEL;
            action.param = PARAM_ONE_WHEEL_RIGHT;
        }
        else if ( color.right == _lastColorLookedFor )
        {
            action.type = ACTION_ONE_WHEEL;
            action.param = PARAM_ONE_WHEEL_LEFT;
        }
    }

    if ( action.type == ACTION_TURN )
    {
        _locatecolor_turnId = action.id;
    }

    if ( action.type == ACTION_STRAIGHT && lastAction.type == ACTION_STRAIGHT )
    {
        action.id = lastAction.id;
    }

    return action;
}
