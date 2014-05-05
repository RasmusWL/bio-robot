#define NUM_COLORS 6

PROGMEM const unsigned char DETECTION_LOW_VALS[NUM_COLORS * 4] =
{
  240, 125,  70, 105, // RED
  155, 205,  90, 105, // GREEN
  155, 215, 235, 135, // BLUE
  240, 240, 240, 220, // WHITE
  217, 200,  80, 162, // ORANGE
  240, 240, 120, 190  // YELLOW
};

PROGMEM const unsigned char DETECTION_HIGH_VALS[NUM_COLORS * 4] =
{
  255, 160, 105, 165, // RED
  190, 255, 125, 155, // GREEN
  185, 245, 255, 185, // BLUE
  255, 255, 255, 255, // WHITE
  255, 255, 110, 187, // ORANGE
  255, 255, 145, 230  // YELLOW
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
