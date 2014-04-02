#include <ADJDS311.h>
#include <Wire.h>
#include <avr/pgmspace.h>

/* MEMORY STUFF
 * http://arduino.cc/en/Tutorial/Memory
 * http://www.arduino.cc/en/Reference/PROGMEM
 */

int sensorLed_pin = 2; //LED on the ADJDS-311
ADJDS311 colorSensor(sensorLed_pin);

/* PIN LAYOUT:
 *
 * ANALOG:
 *   0:  O  Color Sensor Select 1 (DIGITAL)
 *   1:  I  Distance Sensor 1
 *   2:  I  Distance Sensor 2
 *   3:  I  Distance Sensor 3
 *   4:  I  Color Sensor SDA (I2C)
 *   5:  I  Color Sensor SCL (I2C)
 *
 * DIGITAL:
 *   0:     [might be unusable due to serial connection]
 *   1:     [might be unusable due to serial connection]
 *   2:  I  Left Encoder A (interrupt 0)
 *  ~3:  I  Right Encoder A (interrupt 1)
 *   4:  I  Left Encoder B
 *  ~5:  ~  Red LED   [longer duty cycle - http://arduino.cc/en/Reference/AnalogRead]
 *  ~6:  ~  Green LED [longer duty cycle - http://arduino.cc/en/Reference/AnalogRead]
 *   7:  I  Right Encoder B
 *   8:  O  Color Sensor Select 2
 *  ~9:  ~  Blue LED
 * ~10:  ~  Left Motor speed
 * ~11:  ~  Right Motor speed
 *  12:  O  Left Motor dir
 *  13:  O  Right Motor dir
 *
 */

#define RED_PIN 5
#define GREEN_PIN 6
#define BLUE_PIN 9

#define LEFT_MOTOR_SPEED_PIN 10
#define LEFT_MOTOR_DIR_PIN 12

////////////////////////////////////////////////////////////////////////////////

#define NUM_COLORS 6

PROGMEM const unsigned char DETECTION_LOW_VALS[NUM_COLORS * 4] =
{
  230, 142,  70, 112, // RED
  150, 212, 100, 112, // GREEN
  130, 211, 185, 140, // BLUE
  250, 250, 250, 242, // WHITE
  217, 200,  80, 162, // ORANGE
  245, 245, 116, 180  // YELLOW
};

PROGMEM const unsigned char DETECTION_HIGH_VALS[NUM_COLORS * 4] =
{
  255, 171,  91, 133, // RED
  186, 255, 124, 154, // GREEN
  176, 255, 243, 171, // BLUE
  255, 255, 255, 255, // WHITE
  255, 255, 110, 187, // ORANGE
  255, 255, 157, 253  // YELLOW
};

PROGMEM const unsigned char LED_VALS[NUM_COLORS * 3] =
{
  255,   0,   0, // RED
    0, 255,   0, // GREEN
    0,   0, 255, // BLUE
  255, 255, 255, // WHITE
  255, 127,   0, // ORANGE
  255, 255,   0  // YELLOW
};

////////////////////////////////////////////////////////////////////////////////

void setup()
{
  Serial.begin(9600);
  setLED(255,0,0);

  colorSensor.init();
  colorSensor.ledOn(); //turn LED on

  // Calibrate white
  // Need to hold white card in front (1-3mm) of it to calibrate from
  colorSensor.calibrate();

  setLED(0,0,0);
}

////////////////////////////////////////////////////////////////////////////////

bool nop = true;

void loop()
{
  if (Serial.available() > 0)
  {
    char msg = Serial.read();

    if ( msg == 78 ) // N
    {
      nop = !nop;
    }
    else if ( msg == 67 ) // C
    {
      colorSensor.calibrate();
    }
  }

  if (nop) { return; }


  RGBC color = colorSensor.read(); //read the color
  color.red = map(color.red, 0, 1024, 0, 255);
  color.green = map(color.green, 0, 1024, 0, 255);
  color.blue = map(color.blue, 0, 1024, 0, 255);
  color.clear = map(color.clear, 0, 1024, 0, 255);

  int matchedColour = findColorMatch(color);

  if (matchedColour != -1)
  {
    int offset = matchedColour * 3;

    int red   = pgm_read_byte_near(LED_VALS+offset+0);
    int green = pgm_read_byte_near(LED_VALS+offset+1);
    int blue  = pgm_read_byte_near(LED_VALS+offset+2);

    setLED(red,green,blue);
  }
  else
  {
    setLED(0,0,0);
  }

  Serial.print(matchedColour);
  Serial.print(" | ");
  Serial.print(color.red);
  Serial.print(" | ");
  Serial.print(color.green);
  Serial.print(" | ");
  Serial.print(color.blue);
  Serial.print(" | ");
  Serial.println(color.clear);

  delay(300); //just here to slow down the serial output

}

////////////////////////////////////////////////////////////////////////////////

int findColorMatch(RGBC color)
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
