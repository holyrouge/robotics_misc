/*
 * Implements a way to control servo by bitbanging PWM. PWM period is different
 * than normal arduino PWM period.
 * 
 * Author: Sasha Sokolov
 * Date: 9/19/2017
 */
const int servo_pin = 8;

void setup() 
{
  pinMode(8, OUTPUT);
  int commandPulse = 1500;
  for(int i=0; i<70; i++) //50 pulses are about right to make sure that the servo goes to the given position (actual value should be measured)
  {
    digitalWrite(servo_pin, HIGH); //on
    delayMicroseconds(commandPulse); //the command pulse nust be a min of 1ms (0 degrees), and 2ms (180 degrees)
    digitalWrite(servo_pin, LOW); //off
    delayMicroseconds(20000 - commandPulse); //servo period should be 20ms 
  }
}

void loop() 
{

}
