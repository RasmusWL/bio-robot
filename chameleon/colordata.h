#define NUM_COLORS 6

PROGMEM const unsigned char DETECTION_LOW_VALS[NUM_COLORS * 4] =
{
  240, 110,  60, 110, // RED
  130, 170,  70, 105, // GREEN
  125, 190, 190, 130, // BLUE
  240, 240, 210, 190, // WHITE
  217, 200,  80, 162, // ORANGE
  240, 240, 110, 190  // YELLOW
};

PROGMEM const unsigned char DETECTION_HIGH_VALS[NUM_COLORS * 4] =
{
  255, 170, 105, 165, // RED
  174, 255, 115, 154, // GREEN
  175, 255, 240, 170, // BLUE
  255, 255, 255, 255, // WHITE
  255, 255, 110, 187, // ORANGE
  255, 255, 145, 250  // YELLOW
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
