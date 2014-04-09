void avoid_avoid(info_t* info, action_t* action)
{
    // TODO : has the front sensor been adjusted? (as it is positionsed more forwards)

    int proxy0CM = info->proxy0CM;
    int proxy1CM = info->proxy1CM;
    int proxy2CM = info->proxy2CM;

    if ( proxy0CM > 30 && proxy1CM > 30 && proxy2CM > 30 )
    {
        return;
    }

    action->type = ACTION_TURN;

    if ( proxy0CM < proxy1CM-2 && proxy1CM < proxy2CM-2 )
    {
        /* Should turn Right
         *
         *        \
         *  0 =>   \
         *  1 =>    \
         *  2 =>     \
         *            \
         */

        action->param = SMALLEST_TURN;
    }
    else if (proxy2CM < proxy1CM-2 && proxy1CM < proxy0CM-2)
    {
        /* Should turn Left
         *
         *            /
         *  0 =>     /
         *  1 =>    /
         *  2 =>   /
         *        /
         */

        action->param = -SMALLEST_TURN;
    }
    else
    {

        /* Something comepletely else, maybe something like (or opposite)
         *
         *            /
         *  0 =>     /
         *  1 =>    (
         *  2 =>     \
         *            \
         */

        /* Or if sensors are accurate
         *
         *          |
         *  0 =>    |
         *  1 =>    |
         *  2 =>    |
         *          |
         */


        // TODO : take the history into consideration
        if (action->turnPreference == 0)
        {
            if (random(1) == 0) action->turnPreference = -1;
            else                action->turnPreference =  1;
        }

        action->param = action->turnPreference * SMALLEST_TURN;
    }
}
