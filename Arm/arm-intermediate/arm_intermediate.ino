/*
 * SBU Robot Design Team
 * 
 * Implements a way to control MX 12 servo.
 * Hopefully better than quick and dirrty control, but not fully tested yet
 * 
 * Author: Sasha Sokolov and Daniel Moses
 * Date: 4/13/2018                                               PUT RIGHT TIME STAMP
 */

// changed to 8 for intermediate arm control
#define ARM_PACKET_SIZE 8
#define HEADER 0
#define BASE_ARM 1 //  left x axis
#define VERTICAL_TOGGLE 2 // LB
#define VERTICAL 3  // left y axis, elbow or arm depends on LB
#define WRIST_PITCH 4 // right y axis
#define WRIST_ROTATION 5 // right x axis
#define HAND_CONTROL 6 // RB
#define CHECKSUM 7

// byte corresponding to each motor
#define LIN_ACT_1 1 // shoulder
#define SERVO_1 3 // vertical
#define SERVO_2 4 // horizontal
#define SERVO_3 5 // clamp
#define SERVO_4 2 // elbow

// ID's of each servo
#define WRIST_PITCH_ID 1
#define WRIST_ROTATION_ID 2
#define HAND_CLOSE_PIN_ID 3
#define ELBOW_ID 4

// speed of each servo motor
#define SERVO_1_SPEED 50 // speed of servo one(vertical movement of the claw)
#define SERVO_2_SPEED 100 // speed of servo two(roatation of the claw)
#define SERVO_3_SPEED 400 // speed of claw closing
#define SERVO_4_SPEED 100 // speed of elbow movement

// lin_act 1 (lower arm)
#define BASE_ARM_SPEED 4
#define BASE_ARM_DIR1 3
#define BASE_ARM_DIR2 2

#define LINEAR_ACT_MOVESPEED 60
#define CONTINUOUS_ROTATION_SPEED_FORWARDS 300
#define CONTINUOUS_ROTATION_SPEED_BACKWARDS 1024+300

// all speed(and therefore direction) values should be 0-255 so we only need one byte per 

void setup() 
{
  pinMode(BASE_ARM_SPEED, OUTPUT);
  pinMode(BASE_ARM_DIR1, OUTPUT);
  pinMode(BASE_ARM_DIR2, OUTPUT);
  
  //pinMode(FOREARM_SPEED, OUTPUT);
  //pinMode(FOREARM_DIR1, OUTPUT);
  //pinMode(FOREARM_DIR2, OUTPUT);
  
  Serial1.begin(1000000);           //AX Motor             //Preparing Serial1 pin with baud rate of 1000000
  Serial2.begin(1000000);           //AX Motor
  Serial3.begin(115200);    //RADIO                   
  Serial.begin(115200);
  Serial.flush();
}

int index = 0;
char input_data[ARM_PACKET_SIZE];

void loop() 
{
  //Read from radio
  if(Serial3.available()) {
    int data = Serial3.read();
      if(index != 0 && data == 0xFF) {
         //Misaligned... ignore data until we get 0xFF for start of next packet
         index = 0;
         return;
      }
      if(index == 0 && data != 0xFF) {
        //misaligned... throw out data until this is true;
        return;
      }
      input_data[index] = data;
      index++;
  }

  //We got a full packet
  if(index == 7) {

    char sum = 0;
    for(int i = 1; i < 6; i++) {
      sum+= input_data[i];
    }
    sum += 0xBB;
    if(sum == input_data[CHECKSUM]) {
      for(int i = 0; i < ARM_PACKET_SIZE; i++) {
        //Serial.print((unsigned char)input_data[i]);
        //Serial.print(", ");
      }


      
    } else {
      //Serial.println("checksum fail");
    }
    index = 0;
  }
}


void continuousRotation(int id, int dir_sig) {
 if(dir_sig == 0) {
  AX(id, 0);
 } else if (dir_sig == 1) {
    AX(id, CONTINUOUS_ROTATION_SPEED_FORWARDS);
 } else if (dir_sig == 2) {
    AX(id, CONTINUOUS_ROTATION_SPEED_BACKWARDS);
 }
}

// this one allows you to specify a speed for continuous rotation
void continuousRotationSpeed(int id, int dir_sig, int com) {
 if(dir_sig == 0) {
  AX(id, 0);
 } else if (dir_sig == 1) {
    AX(id, com);
 } else if (dir_sig == 2) {
    AX(id, com);
 }
}

void moveLinearAct(int speed_pin, int dir1_pin, int dir2_pin, int dir_sig) {
 if(dir_sig == 0) {
  digitalWrite(dir1_pin, LOW);
  digitalWrite(dir2_pin, LOW);
  digitalWrite(speed_pin, 0);
 } else if (dir_sig == 1) {
    digitalWrite(dir1_pin, LOW);
    digitalWrite(dir2_pin, HIGH);
    digitalWrite(speed_pin, 1);
 } else if (dir_sig == 2) {
    digitalWrite(dir2_pin, LOW);
    digitalWrite(dir1_pin, HIGH);
    digitalWrite(speed_pin, 1);
 }
}

//0 to 4095 for position, sets to this position
void MX(int id, int com)
{
  byte data[8] = {255, 255, id, 5, 3, 30, lowByte(com), highByte(com)};
  sendData(data, sizeof(data));
}

//0 to 2047 for speed (0 to 1023 CCW (0 stop, 1023 max speed), 1024 to 2047 CW (1024 stop, 2047 max speed))
//rotates clockwise or counterclockwise
void AX(int id, int com)
{
  byte data[8] = {255, 255, id, 5, 3, 32, lowByte(com), highByte(com)};
  sendData(data, sizeof(data));
}

void sendData(byte data[], int len)
{
  byte sum = 0;
  for (int i = 2; i < len; i++)
  {
    sum += data[i];                             //Adding up the data values to get checksum
  }
  Serial1.write(data, len);                     //Sending the data
  Serial1.write(~sum);                          //Sending the checksum
  Serial1.end();                                //Disabling Serial1 so servo can respond
  getData();
}
void getData()
{
  while(Serial2.available())
  {
    Serial.println(Serial2.read());             //Reading echo
  }
  while(!Serial2.available()){}                 //Waiting for status packet
  while(Serial2.available())
  {
    Serial.println(Serial2.read());             //Reading status packet
  }
  Serial.println();
  Serial1.begin(1000000);                       //Preparing Serial1 pin with baud rate of 1000000
}
