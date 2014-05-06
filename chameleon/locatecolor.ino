#define MAX_TIME_TO_LOCATE 10 * 1000
#define TIME_AFTER_LAST_COLOR 7 * 1000

int _lastColorLookedFor;
long locatecolor_endTime;

void locatecolor_setup()
{
    _lastColorLookedFor = -2;
    locatecolor_endTime = 0;
}

bool locatecolor_willLookFor(colormatch_t color)
{
    return _locatecolor_getNewColor(color) != -1 && locatecolor_endTime + TIME_AFTER_LAST_COLOR < millis();
}

void locatecolor_switchto( colormatch_t color )
{
    locatecolor_endTime = millis() + MAX_TIME_TO_LOCATE;
    _lastColorLookedFor = _locatecolor_getNewColor(color);
}

int _locatecolor_getNewColor( colormatch_t color )
{
    if ( color.front != -1 && color.front != _lastColorLookedFor )
    {
        return color.front;
    }
    else if ( color.left != -1 && color.left != _lastColorLookedFor )
    {
        return color.left;
    }
    else if ( color.right != -1 && color.right != _lastColorLookedFor )
    {
        return color.right;
    }
}

bool locatecolor_finished()
{
    return millis() > locatecolor_endTime;
}

bool locatecolor_success(colormatch_t color)
{
    return color.front == color.left && color.front == color.right && color.front != -1;
}

int _locatecolor_turnId;

action_t locatecolor_newAction(action_t lastAction, colormatch_t color)
{
    action_t action = {};
    action.id = action_new_id();

    if ( lastAction.type == ACTION_TURN && lastAction.id == _locatecolor_turnId )
    {
        if ( color.front == -1 && action_degTurned(lastAction.param) < 360 )
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

    if ( color.front == color.left && color.front == color.right && color.front != -1 )
    {
        action.type = ACTION_STOP;
        action.param = 0;
        return action;
    }

    if ( color.front == -1)
    {
        if ( color.left != -1 && color.right != -1 )
        {
            action.type = ACTION_TURN;
            action.param = TURN_WAY_LEFT;
        }
        else if ( color.left != -1 )
        {
            action.type = ACTION_TURN;
            action.param = TURN_WAY_LEFT;
        }
        else if ( color.right != -1 )
        {
            action.type = ACTION_TURN;
            action.param = TURN_WAY_RIGHT;
        }
    }
    else
    {
        if ( color.left == -1 && color.right == -1 )
        {
            action.type = ACTION_STRAIGHT;
            action.param = 1000;
        }
        else if ( color.left != -1 )
        {
            action.type = ACTION_ONE_WHEEL;
            action.param = PARAM_ONE_WHEEL_RIGHT;
        }
        else if ( color.right != -1 )
        {
            action.type = ACTION_ONE_WHEEL;
            action.param = PARAM_ONE_WHEEL_LEFT;
        }
    }

    if ( action.type == ACTION_TURN )
    {
        _locatecolor_turnId = action.id;
    }

    return action;
}
