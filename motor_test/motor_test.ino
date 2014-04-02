// motor_test.ino

#define LEFT_MOTOR_SPEED_PIN 10
#define LEFT_MOTOR_DIR_PIN 12

void setup()
{
    analogWrite(LEFT_MOTOR_SPEED_PIN, 127);
    digitalWrite(LEFT_MOTOR_DIR_PIN, HIGH);
}

bool leftIsForwards = true;

void loop()
{
    delay(5000);

    leftIsForwards = !leftIsForwards;

    if(leftIsForwards)
    {
        digitalWrite(LEFT_MOTOR_DIR_PIN, HIGH);
    }
    else
    {
        digitalWrite(LEFT_MOTOR_DIR_PIN, LOW);
    }
}

