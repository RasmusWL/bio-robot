struct action_t
{
    char type;
    int param;
    int id;
};

struct robot_info_t
{
    float x;
    float y;
    int angle;
};

#define ACTION_STRAIGHT 0
// param = max number of CM

#define ACTION_TURN 1
// param = way to turn
#define TURN_WAY_LEFT -1;
#define TURN_WAY_RIGHT 1;

#define ACTION_STOP 2
// param = delay in miliseconds

#define ACTION_ONE_WHEEL 3
#define PARAM_ONE_WHEEL_LEFT 0
#define PARAM_ONE_WHEEL_RIGHT 1
// param = which wheel (0 for left, 1 for right)



#define HISTORY_LENGTH 8
action_t action_history[HISTORY_LENGTH];
