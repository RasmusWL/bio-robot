volatile int leftCount;
volatile int rightCount;

void encoder_setup()
{
    pinMode(ENCODER_LEFT_A, INPUT);
    pinMode(ENCODER_LEFT_B, INPUT);

    pinMode(ENCODER_RIGHT_A, INPUT);
    pinMode(ENCODER_RIGHT_B, INPUT);

    leftCount = 0;
    rightCount = 0;

    attachInterrupt(0, leftEncoderEvent, CHANGE);
    attachInterrupt(1, rightEncoderEvent, CHANGE);
}

void leftEncoderEvent()
{
    leftCount++;
}

void rightEncoderEvent()
{
    rightCount++;
}
