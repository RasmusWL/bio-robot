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
