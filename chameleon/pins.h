/* PIN LAYOUT:
 *
 * ANALOG:
 *   0:  O  LED power on/off
 *   1:  I  Proximity Sensor 1
 *   2:  I  Proximity Sensor 2
 *   3:  I  Proximity Sensor 3
 *   4:  I  Color Sensor SDA (I2C)
 *   5:  I  Color Sensor SCL (I2C) - clock
 *
 * DIGITAL:
 *   0:     [is unusable due to serial connection]
 *   1:     [is unusable due to serial connection]
 *   2:  I  Left Encoder A (interrupt 0)
 *  ~3:  I  Right Encoder A (interrupt 1)
 *   4:  I  Left/Right Encoder B (only one encoder has a working B output)
 *  ~5:  ~  Red LED   [longer duty cycle - http://arduino.cc/en/Reference/AnalogRead]
 *  ~6:  ~  Green LED [longer duty cycle - http://arduino.cc/en/Reference/AnalogRead]
 *   7:  O  Color Sensor Select 0
 *   8:  O  Color Sensor Select 1
 *  ~9:  ~  Blue LED
 * ~10:  ~  Left Motor speed
 * ~11:  ~  Right Motor speed
 *  12:  O  Left Motor dir
 *  13:  O  Right Motor dir
 */

#define COLOR_SENS_SELECT_0_PIN 7
#define COLOR_SENS_SELECT_1_PIN 8

#define PROX_0_PIN A3
#define PROX_1_PIN A1
#define PROX_2_PIN A2

#define LED_ON_PIN A0
#define RED_PIN 5
#define GREEN_PIN 6
#define BLUE_PIN 9

#define ENCODER_LEFT_A  3
#define ENCODER_LEFT_B  4

#define ENCODER_RIGHT_A  2
//#define ENCODER_RIGHT_B  7

#define LEFT_MOTOR_SPEED_PIN 10
#define LEFT_MOTOR_DIR_PIN 12

#define RIGHT_MOTOR_SPEED_PIN 11
#define RIGHT_MOTOR_DIR_PIN 13
