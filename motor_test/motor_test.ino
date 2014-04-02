#define LEFT_MOTOR_SPEED_PIN 10
#define LEFT_MOTOR_DIR_PIN 12

void setup()
{
    Serial.begin(9600);

    pinMode(LEFT_MOTOR_SPEED_PIN, OUTPUT);
    pinMode(LEFT_MOTOR_DIR_PIN, OUTPUT);

    analogWrite(LEFT_MOTOR_SPEED_PIN, 255);
    digitalWrite(LEFT_MOTOR_DIR_PIN, LOW);
}

bool leftIsForwards = true;

void loop()
{
    if (Serial.available() > 0)
    {
        char msg = Serial.read();

        if ( msg == 83 ) // S
        {
            int newSpeed = Serial.parseInt();
            analogWrite(LEFT_MOTOR_SPEED_PIN, newSpeed);
        }
        else if ( msg == 67 ) // C
        {
            leftIsForwards = !leftIsForwards;
            if (leftIsForwards)
            {
                digitalWrite(LEFT_MOTOR_DIR_PIN, HIGH);
            }
            else
            {
                digitalWrite(LEFT_MOTOR_DIR_PIN, LOW);
            }
        }
    }
    delay(1000);
}

