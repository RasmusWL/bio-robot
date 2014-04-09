#define HISTORY_LENGTH 8

char actionHistory[HISTORY_LENGTH];
char paramHistory[HISTORY_LENGTH];

struct info_t
{
    int proxy0CM;
    int proxy1CM;
    int proxy2CM;

    int color0Match;
    int color1Match;
    int color2Match;
};

struct action_t
{
    char type;
    char param;
    char turnPreference;
};

#define ACTION_STRAIGHT 0
// param = none

#define ACTION_TURN     1
// param = degrees


#define SMALLEST_TURN 15

#define STATE_ROAMING  0
#define STATE_CLR_LOOK 1
