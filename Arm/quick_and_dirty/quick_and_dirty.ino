/*
 * SBU Robot Design Team
 * 
 * Implements a way to control MX 12 servo.
 * 
 * Author: Sasha Sokolov
 * Date: 10/9/2017                                                PUT RIGHT TIME STAMP
 */

// changed to 11 for quick and dirty control
#define ARM_PACKET_SIZE 7
#define HEADER 0
#define BASE_ARM 1 //  D left, dright
#define FOREARM 2  // Dup, Ddown
#define WRIST_PITCH 3 // LT , RT
#define WRIST_ROTATION 4 // LB, RB
#define HAND_CLOSE 5 //A/B
#define CHECKSUM 6 

#define LIN_ACT_1 1 // lower linear actuator
#define LIN_ACT_2 2 // upper linear actuator
#define SERVO_1 3 // vertical
#define SERVO_2 4 // horizontal
#define SERVO_3 5 // clamp

#define LB 1 // left bumper
#define LT 2 // left trggier
#define RB 3 // right bumper
#define RT 4 // right trigger
#define X 5 // x button
#define Y 7 // y button
#define A 8 // a button
#define B 9 // b button
#define LEFT_STICK 10 // left stick button
#define RIGHT_STICK 11 // right stick button

/*
 * QUICK AND DIRTY CONTROL SCHEME
 * 
 * LB: LINACT_1 (lower liinear actuator) up
 * LT: LINACT_1 (lower linaer actuator) down
 * 
 * RB: LINACT_2 (upper linear actuator) up
 * RT: LINACT_2 (upper linear actuator) down
 * 
 * X: SERVO_2 (horizontal servo) backward
 * B: SERVO_2 (horizontal servo) forward
 * 
 * Y: SERVO_1 (vertical servo) forward
 * A: SERVO_1 (vertical servo) backward
 * 
 * LEFT_STICK: unclamp(open hand)
 * RIGHT_STICK clamp(close hand)
 */


// lin_act 1 (lower arm)
#define BASE_ARM_SPEED 4
#define BASE_ARM_DIR1 3
#define BASE_ARM_DIR2 2

// lin_act 2 (uuper arm)
#define FOREARM_SPEED 7
#define FOREARM_DIR1 6
#define FOREARM_DIR2 5

#define WRIST_PITCH_ID 1
#define WRIST_ROTATION_ID 2
#define HAND_CLOSE_PIN_ID 3

#define LINEAR_ACT_MOVESPEED 60
#define CONTINUOUS_ROTATION_SPEED_FORWARDS 300
#define CONTINUOUS_ROTATION_SPEED_BACKWARDS 1024+300

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
    //Serial.println((int) data);
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
      //Serial.println();

     // moveLinearAct(BASE_ARM_SPEED, BASE_ARM_DIR1, BASE_ARM_DIR2, input_data[BASE_ARM]);
//      moveLinearAct(FOREARM_SPEED, FOREARM_DIR1, FOREARM_DIR2, input_data[FOREARM]);
//      //manual_move(WRIST_PITCH_ID, WRIST_PITCH_PIN, input_data[WRIST_PITCH]);
//      continuousRotation(WRIST_ROTATION_ID, input_data[WRIST_ROTATION]);
//      continuousRotation(HAND_CLOSE_PIN_ID, input_data[HAND_CLOSE]);

    if (input_data[LIN_ACT_1] == 1) {
      moveLinearAct(BASE_ARM_SPEED, BASE_ARM_DIR1, BASE_ARM_DIR2, 1);
    } else if (input_data[LIN_ACT_1] == 2) {
      moveLinearAct(BASE_ARM_SPEED, BASE_ARM_DIR1, BASE_ARM_DIR2, 2);
    } else {
      moveLinearAct(BASE_ARM_SPEED, BASE_ARM_DIR1, BASE_ARM_DIR2, 0);
    }

    if (input_data[LIN_ACT_2] == 1) {
      moveLinearAct(FOREARM_SPEED, FOREARM_DIR1, FOREARM_DIR2, 1);
    } else if (input_data[LIN_ACT_1] == 2) {
      moveLinearAct(FOREARM_SPEED, FOREARM_DIR1, FOREARM_DIR2, 2);
    } else {
      moveLinearAct(FOREARM_SPEED, FOREARM_DIR1, FOREARM_DIR2, 0);
    }

    continuousRotation(WRIST_PITCH, input_data[SERVO_1]);
    continuousRotation(WRIST_ROTATION, input_data[SERVO_2]);
    continuousRotation(HAND_CLOSE, input_data[SERVO_3]);
      
      
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
