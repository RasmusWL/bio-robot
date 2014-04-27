#define WHEEL_CIRCUMFERENCE 14.137 // cm
#define WHEEL_TICKS 155            // The number of 'ticks' for a full wheel cycle

// 155 seems to work! for only rising ;)

// 12 encoder ticks per motor reveloution
// 51.45 : 1 gear ratio

void encoder_setup()
{
    pinMode(ENCODER_LEFT_A, INPUT);

    pinMode(ENCODER_RIGHT_A, INPUT);

    encoder_reset();

    attachInterrupt(0, leftEncoderEvent, RISING);
    attachInterrupt(1, rightEncoderEvent, RISING);
}

void encoder_reset()
{
    leftCount = 0;
    rightCount = 0;
}

float encoder_distance_traveled()
{
    // TODO : under the assumption that both motors and encoders work the same

    float avg = (leftCount + rightCount) / 2;
    return (avg * WHEEL_CIRCUMFERENCE) / WHEEL_TICKS;
}

void leftEncoderEvent()
{
    leftCount++;
}

void rightEncoderEvent()
{
    rightCount++;
}
