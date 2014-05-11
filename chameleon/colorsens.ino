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

    colorsens_setCalibration(0,360,110,2,0,0,5);
    colorsens_setCalibration(1,185,60,1,3,0,5);
    colorsens_setCalibration(2,170,62,0,2,1,5);

    colorsens_off();
}

void colorsens_calibrate()
{
    colorsens_activate(&colorSensor0);
    colorSensor0.init();
    colorSensor0.calibrate();
    Serial.println("Done calibrate 0");

    colorsens_activate(&colorSensor1);
    colorSensor1.init();
    colorSensor1.calibrate();
    Serial.println("Done calibrate 1");

    colorsens_activate(&colorSensor2);
    colorSensor2.init();
    colorSensor2.calibrate();
    Serial.println("Done calibrate 2");
}

void colorsens_activate_showoff()
{
    int delay_time = 1000;

    Serial.println("Activating 0");
    colorsens_activate(&colorSensor0);
    delay(delay_time);

    Serial.println("Activating 1");
    colorsens_activate(&colorSensor1);
    delay(delay_time);

    Serial.println("Activating 2");
    colorsens_activate(&colorSensor2);
    delay(delay_time);

    Serial.println("Activating 3");
    colorsens_activate(NULL);
}

void colorsens_activate_special(int num)
{
    if      (num == 0) { colorsens_activate(&colorSensor0); }
    else if (num == 1) { colorsens_activate(&colorSensor1); }
    else if (num == 2) { colorsens_activate(&colorSensor2); }
    else               { colorsens_activate(NULL); }
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

    delay(7);

    currently_active = sens;
}

RGBC colorsens_read(int num)
{
    ADJDS311* sens;

    if      (num == 0) { sens = &colorSensor0; }
    else if (num == 1) { sens = &colorSensor1; }
    else if (num == 2) { sens = &colorSensor2; }

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
    RGBC color = colorsens_read(num);
    return colorsens_findColorMatch(color);
}

void colorsens_debug()
{
    Serial.println("Color sensors debug");
    Serial.print("s0 = ");
    colorsens_debug_sens(0);

    Serial.print("s1 = ");
    colorsens_debug_sens(1);

    Serial.print("s2 = ");
    colorsens_debug_sens(2);

    colorsens_off();
}

void colorsens_off()
{
    colorsens_activate(NULL);
}


void colorsens_debug_sens(int num)
{
    RGBC color = colorsens_read(num);
    int match = colorsens_findColorMatch(color);

    if (match != -1) { Serial.print(" "); }
    Serial.print(match);
    Serial.print(" ~ ");
    print3Digit(color.red);
    Serial.print(" ");
    print3Digit(color.green);
    Serial.print(" ");
    print3Digit(color.blue);
    Serial.print(" ");
    print3Digit(color.clear);
    Serial.print("\n");
}

void colorsens_printCalibration(int num)
{
    ADJDS311* sens;
    if      (num == 0) { sens = &colorSensor0; }
    else if (num == 1) { sens = &colorSensor1; }
    else if (num == 2) { sens = &colorSensor2; }

    colorsens_activate( sens );

    sens->printCalibration();
}

void colorsens_setCalibration(int num, int colorGain, int clearGain,
                              int colorCapRed, int colorCapGreen,
                              int colorCapBlue, int colorCapClear)
{
    ADJDS311* sens;
    if      (num == 0) { sens = &colorSensor0; }
    else if (num == 1) { sens = &colorSensor1; }
    else if (num == 2) { sens = &colorSensor2; }

    colorsens_activate( sens );

    sens->setCalibration(colorGain,clearGain, colorCapRed, colorCapGreen, colorCapBlue, colorCapClear);
}

colormatch_t colorsens_measureAll()
{
    colormatch_t match;

    match.left = colorsens_measure(2);
    match.front = colorsens_measure(0);
    match.right = colorsens_measure(1);

    colorsens_off();
    return match;
}

void colorsens_readALot(int fromSens, int toSens)
{
    int minR = 255;
    int minG = 255;
    int minB = 255;
    int minC = 255;

    int maxR = 0;
    int maxG = 0;
    int maxB = 0;
    int maxC = 0;

    for (int i = 0; i < 15; i ++)
    {
        for (int j = fromSens; j <= toSens; j++)
        {
            RGBC color = colorsens_read(j);
            // int match = colorsens_findColorMatch(color);

            minR = min(minR, color.red);
            minG = min(minG, color.green);
            minB = min(minB, color.blue);
            minC = min(minC, color.clear);

            maxR = max(maxR, color.red);
            maxG = max(maxG, color.green);
            maxB = max(maxB, color.blue);
            maxC = max(maxC, color.clear);
        }

        delay(10);
    }

    print3Digit(minR);
    Serial.print(" ");
    print3Digit(minG);
    Serial.print(" ");
    print3Digit(minB);
    Serial.print(" ");
    print3Digit(minC);
    Serial.print("\n");

    print3Digit(maxR);
    Serial.print(" ");
    print3Digit(maxG);
    Serial.print(" ");
    print3Digit(maxB);
    Serial.print(" ");
    print3Digit(maxC);
    Serial.print("\n");
}
