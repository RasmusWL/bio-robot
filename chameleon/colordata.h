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
