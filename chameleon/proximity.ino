// returns distance in cm (not 100% percicse)
int measureProxSensor (int pin)
{
    int val = analogRead(pin);

    if (val <= 80)
    {
        return 80;
    }

    // formula taken from http://www.acroname.com/articles/linearizing-sharp-ranger.html
    return ( 6787 / (val-3) ) - 4;
}

prox_t prox_readAll()
{
    prox_t reading = {};

    reading.left = measureProxSensor(PROX_0_PIN);
    reading.middle = measureProxSensor(PROX_1_PIN);
    reading.right = measureProxSensor(PROX_2_PIN);

    return reading;
}
prox_t prox_all_averaged(int samples, int delayTime)
{
    // long startTime = millis();

    long leftDist = 0;
    long middleDist = 0;
    long rightDist = 0;

    for (int i = 0; i < samples; i++)
    {
        prox_t reading = prox_readAll();

        leftDist += reading.left;
        middleDist += reading.middle;
        rightDist += reading.right;

        delay(delayTime);
    }

    leftDist = leftDist / samples;
    middleDist = middleDist / samples;
    rightDist = rightDist / samples;

    prox_t result = {};
    result.left = leftDist;
    result.middle = middleDist;
    result.right = rightDist;

    // long endTime = millis();

    Serial.print("totals:");
    print3Digit(leftDist);
    print3Digit(middleDist);
    print3Digit(rightDist);

    // Serial.print (" [");
    // Serial.print(endTime - startTime);
    // Serial.print("]");
    Serial.print("\n");

    return result;
}

