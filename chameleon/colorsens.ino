ADJDS311 colorSensor0;
ADJDS311 colorSensor1;
ADJDS311 colorSensor2;

ADJDS311* currently_active = NULL;

/* These colour sensors all use the same I2C address,
 * meaning tricks must be used to talk with 3.
 */

void colorsens_setup()
{
    pinMode(COLOR_SENS_SELECT_0_PIN, OUTPUT);
    pinMode(COLOR_SENS_SELECT_1_PIN, OUTPUT);

    colorsens_activate(&colorSensor0);
    colorSensor0.init();
    colorSensor0.calibrate();

    colorsens_activate(&colorSensor1);
    colorSensor1.init();
    colorSensor1.calibrate();

    colorsens_activate(&colorSensor2);
    colorSensor2.init();
    colorSensor2.calibrate();
}

void colorsens_activate(ADJDS311* sens)
{
    if (currently_active == sens) return;

    // TODO: logic
    Serial.println("colorsens_activate: TODO logic");

    //COLOR_SENS_SELECT_1_PIN
    //COLOR_SENS_SELECT_2_PIN

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

