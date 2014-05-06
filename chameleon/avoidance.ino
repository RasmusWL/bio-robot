bool avoidance_needToStop(prox_t prox)
{
    return ( prox.middle < DIST_MIDDLE_STOP || prox.right < DIST_SIDE_STOP || prox.left < DIST_SIDE_STOP );
}

bool avoidance_shouldNotGoThisWay(prox_t prox)
{
    return ( prox.middle < DIST_MIDDLE_MEDIUM || prox.right < DIST_MIDDLE_MEDIUM || prox.left < DIST_MIDDLE_MEDIUM );
}

bool avoidance_shouldNotGoThisWayMiddleOnly(prox_t prox)
{
    return ( prox.middle < DIST_MIDDLE_MEDIUM || prox.right < DIST_SIDE_STOP || prox.left < DIST_SIDE_STOP );
}

int avoidance_turnPref(prox_t prox)
{
    if ( prox.right > DIST_SIDE_MEDIUM )
    {
        return TURN_WAY_RIGHT;
    }
    else if ( prox.left > DIST_SIDE_MEDIUM )
    {
        return TURN_WAY_LEFT;
    }

    if (random(0, 2) == 0) { return TURN_WAY_LEFT; }
    else                   { return TURN_WAY_RIGHT; }
}
