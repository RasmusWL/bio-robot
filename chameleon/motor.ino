#define FORWARDS HIGH
#define BACKWARDS LOW

int _leftSpeed = 0;
int _rightSpeed = 0;

void motor_setup()
{
    pinMode(LEFT_MOTOR_SPEED_PIN, OUTPUT);
    pinMode(LEFT_MOTOR_DIR_PIN, OUTPUT);

    pinMode(RIGHT_MOTOR_SPEED_PIN, OUTPUT);
    pinMode(RIGHT_MOTOR_DIR_PIN, OUTPUT);

    motor_setLeftSpeed(0);
    motor_setRightSpeed(0);
}

void motor_setSpeeds(int leftSpeed, int rightSpeed)
{
    motor_setLeftSpeed(leftSpeed);
    motor_setRightSpeed(rightSpeed);
}

void motor_setLeftSpeed(int newSpeed)
{
    newSpeed = constrain(newSpeed, -255, 255);

    __setMotorSpeed(newSpeed, _leftSpeed, LEFT_MOTOR_SPEED_PIN, LEFT_MOTOR_DIR_PIN);
    _leftSpeed = newSpeed;
}

void motor_setRightSpeed(int newSpeed)
{
    newSpeed = -newSpeed;
    newSpeed = constrain(newSpeed, -255, 255);

    __setMotorSpeed(newSpeed, _rightSpeed, RIGHT_MOTOR_SPEED_PIN, RIGHT_MOTOR_DIR_PIN);
    _rightSpeed = newSpeed;
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

    analogWrite(SPEED_PIN, abs(newSpeed) );
}

void motor_stop()
{
    if ( _leftSpeed != 0 )
        motor_setLeftSpeed ( (_leftSpeed/abs(_leftSpeed) ) * -255 );

    if ( _rightSpeed != 0 )
        motor_setRightSpeed ( (_rightSpeed/abs(_rightSpeed) ) * 255 );

    delay(20);

    motor_setSpeeds(0,0);
}
