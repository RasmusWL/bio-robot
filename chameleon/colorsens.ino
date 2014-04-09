ADJDS311 colorSensor0;
ADJDS311 colorSensor1;
ADJDS311 colorSensor2;

ADJDS311* currently_active = NULL;

/* These colour sensors all use the same I2C address,
 * meaning tricks must be used to talk with 3.
 * We use the sleep pin of the board, and a multiplexer
 * that will send LOW though to the board that must be active
 */

void colorsens_setup()
{
    Wire.begin();

    pinMode(COLOR_SENS_SELECT_0_PIN, OUTPUT);
    pinMode(COLOR_SENS_SELECT_1_PIN, OUTPUT);

    colorsens_activate(&colorSensor0);
    colorSensor0.init();
    colorSensor0.calibrate();

    colorsens_activate(&colorSensor1);
    colorSensor1.init();
    colorSensor1.calibrate();

    // TODO : activate last color sensor
    // colorsens_activate(&colorSensor2);
    // colorSensor2.init();
    // colorSensor2.calibrate();
}

void colorsens_activate(ADJDS311* sens)
{
    if (currently_active == sens) return;

    if (sens == &colorSensor0)
    {
        digitalWrite(COLOR_SENS_SELECT_0_PIN, LOW);
        digitalWrite(COLOR_SENS_SELECT_1_PIN, LOW);
    }
    else if (sens == &colorSensor1)
    {
        digitalWrite(COLOR_SENS_SELECT_0_PIN, HIGH);
        digitalWrite(COLOR_SENS_SELECT_1_PIN, LOW);
    }
    else if (sens == &colorSensor2)
    {
        digitalWrite(COLOR_SENS_SELECT_0_PIN, LOW);
        digitalWrite(COLOR_SENS_SELECT_1_PIN, HIGH);
    }
    else
    {
        digitalWrite(COLOR_SENS_SELECT_0_PIN, HIGH);
        digitalWrite(COLOR_SENS_SELECT_1_PIN, HIGH);
    }

    delay(3);

    currently_active = sens;
}

RGBC colorsens_read(ADJDS311* sens)
{
    colorsens_activate(sens);

    RGBC color = sens->read();
    color.red = map(color.red, 0, 1024, 0, 255);
    color.green = map(color.green, 0, 1024, 0, 255);
    color.blue = map(color.blue, 0, 1024, 0, 255);
    color.clear = map(color.clear, 0, 1024, 0, 255);

    return color;
}

int colorsens_findColorMatch(RGBC color)
{
    int matchedColour = -1;

    for (int i = 0; i < NUM_COLORS; i++)
    {
        int offset = i * 4;

        RGBC lowColor;
        lowColor.red   = pgm_read_byte_near(DETECTION_LOW_VALS+offset+0);
        lowColor.green = pgm_read_byte_near(DETECTION_LOW_VALS+offset+1);
        lowColor.blue  = pgm_read_byte_near(DETECTION_LOW_VALS+offset+2);
        lowColor.clear = pgm_read_byte_near(DETECTION_LOW_VALS+offset+3);

        RGBC highColor;
        highColor.red   = pgm_read_byte_near(DETECTION_HIGH_VALS+offset+0);
        highColor.green = pgm_read_byte_near(DETECTION_HIGH_VALS+offset+1);
        highColor.blue  = pgm_read_byte_near(DETECTION_HIGH_VALS+offset+2);
        highColor.clear = pgm_read_byte_near(DETECTION_HIGH_VALS+offset+3);

        if ( lowColor.red <= color.red  && lowColor.green <= color.green  && lowColor.blue <= color.blue  && lowColor.clear <= color.clear
          && color.red <= highColor.red && color.green <= highColor.green && color.blue <= highColor.blue && color.clear <= highColor.clear
           )
        {
            matchedColour = i;
        }
    }

    return matchedColour;
}

int colorsens_measure(int num)
{
    ADJDS311* sens;

    if      (num == 0) { sens = &colorSensor0; }
    else if (num == 1) { sens = &colorSensor1; }
    else if (num == 2) { sens = &colorSensor2; }

    RGBC color = colorsens_read(sens);
    return colorsens_findColorMatch(color);
}

void colorsens_debug()
{
    Serial.print("s0 = ");
    colorsens_debug_sens(&colorSensor0);

    Serial.print("s1 = ");
    colorsens_debug_sens(&colorSensor1);

    // TODO : activate last color sensor
    // Serial.print("s2 = ");
    // colorsens_debug_sens(&colorSensor2);
}

void colorsens_debug_sens(ADJDS311* sens)
{
    RGBC color = colorsens_read(sens);
    int match = colorsens_findColorMatch(color);

    if (match != -1) Serial.print(" ")
    Serial.print(match)
    Serial.print(" <~~ ")
    print3Digit(color.red);
    Serial.print(" ");
    print3Digit(color.green);
    Serial.print(" ");
    print3Digit(color.blue);
    Serial.print(" ");
    print3Digit(color.clear);
    Serial.print("\n");
}

void print3Digit(int num)
{
    if (num < 10)
    {
        Serial.print("  ");
    }
    else if (num < 100)
    {
        Serial.print(" ");
    }

    Serial.print(num);
}
