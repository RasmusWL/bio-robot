#define PROX_1_PIN A1

void setup()
{
    Serial.begin(9600);
}


void loop()
{
    int cmDist = measureProxSensor(PROX_1_PIN);

    Serial.print(cmDist);
    Serial.print("\n");

    delay(500);
}

int measureProxSensor (int pin)
{
    int val = analogRead(PROX_1_PIN);

    if (val <= 80)
    {
        return 80;
    }

    // formula taken from http://www.acroname.com/articles/linearizing-sharp-ranger.html
    return ( 6787 / (val-3) ) - 4;
}
