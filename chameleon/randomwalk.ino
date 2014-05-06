action_t randomwalk_newAction(action_t lastAction, prox_t prox)
{
    action_t action = {};
    action.id = action_new_id();

    if ( lastAction.type == ACTION_STRAIGHT )
    {
        action.id = lastAction.id;
    }
    else if ( ( lastAction.type == ACTION_STOP || lastAction.type == ACTION_TURN )
              && avoidance_shouldNotGoThisWay(prox)
            )
    {
        action.type = ACTION_TURN;
        if ( lastAction.type == ACTION_TURN )
        {
            action.param = lastAction.param;
            action.id = lastAction.id;
        }
        else
        {
            action.param = 45 * avoidance_turnPref(prox);
        }

        return action;
    }

    action.type = ACTION_STRAIGHT;

    return action;
}

void randomwalk_switchto()
{

}

// bool findDrivingAction(char* action, char* turnPref, int leftProx, int middleProx, int rightProx, char level)
// {
//     bool changed = false;
//     *action = ACTION_STRAIGHT;
//     *turnPref = PREF_NONE;

//     // ------------------------------------------------------------ //

//     bool middleStop = middleProx < DIST_MIDDLE_STOP;
//     bool leftStop = leftProx < DIST_SIDE_STOP;
//     bool rightStop = rightProx < DIST_SIDE_STOP;

//     bool middleMedium = middleProx < DIST_MIDDLE_MEDIUM;
//     bool leftMedium = leftProx < DIST_SIDE_MEDIUM;
//     bool rightMedium = rightProx < DIST_SIDE_MEDIUM;

//     bool middleSmall = middleProx < DIST_MIDDLE_SMALL;
//     bool leftSmall = leftProx < DIST_SIDE_SMALL;
//     bool rightSmall = rightProx < DIST_SIDE_SMALL;

//     bool middleTiny = middleProx < DIST_MIDDLE_TINY;

//     // ------------------------------------------------------------ //
//     // STOP, we're about to crash!
//     // ------------------------------------------------------------ //

//     if ( leftStop || middleStop || rightStop )
//     {
//         *action = ACTION_TURN;
//         changed = true;

//         if ( leftStop && rightStop )
//         {
//             *turnPref = PREF_180;
//         }
//         else if ( leftStop && !rightMedium ) // maybe extend with `leftStop && rightMedium`
//         {
//             *turnPref = PREF_RIGHT;
//         }
//         else if ( rightStop && !leftMedium )
//         {
//             *turnPref = PREF_LEFT;
//         }
//         else
//         {
//             *turnPref = PREF_NONE;
//         }
//     }

//     if (changed) { goto findDrivingAction_end; }

//     // ------------------------------------------------------------ //
//     // Hey, we're getting pretty close to that thing, move!
//     // ------------------------------------------------------------ //

//     if ( leftMedium && rightMedium && !middleMedium )
//     {
//         // there might just be a hole for us
//     }
//     else if ( leftMedium && (middleMedium || middleSmall) )
//     {
//         *action = ACTION_DIV_MEDIUM;
//         *turnPref = PREF_RIGHT;
//         changed = true;
//     }
//     else if ( rightMedium && (middleMedium || middleSmall) )
//     {
//         *action = ACTION_DIV_MEDIUM;
//         *turnPref = PREF_LEFT;
//         changed = true;
//     }
//     else if ( middleMedium ) // we want this case here, as `leftMedium && middleMedium` should do the same as `leftMedium`
//     {
//         *action = ACTION_DIV_MEDIUM;
//         *turnPref = PREF_NONE;
//         changed = true;
//     }

//     if (changed) { goto findDrivingAction_end; }

//     // ------------------------------------------------------------ //
//     // Oh, there is something nearby, lets do a little turn away
//     // ------------------------------------------------------------ //

//     else if ( leftSmall && rightSmall && !middleSmall )
//     {
//         // there might just be a hole for us
//     }
//     else if ( leftSmall && (middleSmall || middleTiny) )
//     {
//         *action = ACTION_DIV_SMALL;
//         *turnPref = PREF_RIGHT;
//         changed = true;
//     }
//     else if ( rightSmall && (middleSmall || middleTiny) )
//     {
//         *action = ACTION_DIV_SMALL;
//         *turnPref = PREF_LEFT;
//         changed = true;
//     }
//     else if ( middleSmall ) // we want this case here, as `leftMedium && middleMedium` should do the same as `leftMedium`
//     {
//         *action = ACTION_DIV_SMALL;
//         *turnPref = PREF_NONE;
//         changed = true;
//     }

// findDrivingAction_end:

//     return changed;
// }
