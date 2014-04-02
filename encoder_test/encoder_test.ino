// http://www.psurobotics.org/wiki/index.php?title=Wheel_Encoders

#define LEFT_CLOCK A1
#define LEFT_DIR A0

#define LEFT_MOTOR_SPEED_PIN 10
#define LEFT_MOTOR_DIR_PIN 12

volatile unsigned int LeftCount;

void setup()
{
  Serial.begin(9600);

  attachInterrupt(0, LeftEncoderEvent, CHANGE);

  analogWrite(LEFT_MOTOR_SPEED_PIN, 0);
  digitalWrite(LEFT_MOTOR_DIR_PIN, HIGH);

}

void loop()
{
  int oldCount = LeftCount;
  delay(10);
  Serial.print( analogRead(LEFT_DIR) );
  Serial.print(" ");
  Serial.print( analogRead(LEFT_CLOCK) );
  Serial.print (" ");
  Serial.print( LeftCount );
  Serial.print ("\n" );

}

void LeftEncoderEvent()
{
  LeftCount++;
}
