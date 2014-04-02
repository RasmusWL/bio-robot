#define FORWARDS HIGH
#define BACKWARDS LOW

int leftSpeed = 0;
int rightSpeed = 0;

void motor_setup()
{
    pinMode(LEFT_MOTOR_SPEED_PIN, OUTPUT);
    pinMode(LEFT_MOTOR_DIR_PIN, OUTPUT);

    pinMode(RIGHT_MOTOR_SPEED_PIN, OUTPUT);
    pinMode(RIGHT_MOTOR_DIR_PIN, OUTPUT);

    setLeftSpeed(0);
    setRightSpeed(0);
}

void setLeftSpeed(int newSpeed)
{
    __setMotorSpeed(newSpeed, leftSpeed, LEFT_MOTOR_SPEED_PIN, LEFT_MOTOR_DIR_PIN);
    leftSpeed = newSpeed;
}

void setRightSpeed(int newSpeed)
{
    __setMotorSpeed(newSpeed, rightSpeed, RIGHT_MOTOR_SPEED_PIN, RIGHT_MOTOR_DIR_PIN);
    rightSpeed = newSpeed;
}

void __setMotorSpeed(int newSpeed, int oldSpeed, char SPEED_PIN, char DIR_PIN)
{
    if ( 0 <= oldSpeed && 0 > newSpeed )
    {
        digitalWrite(DIR_PIN, BACKWARDS);
    }
    else if (0 >= oldSpeed && 0 < newSpeed )
    {
        digitalWrite(DIR_PIN, FORWARDS);
    }

    analogWrite(SPEED_PIN, newSpeed);
}
