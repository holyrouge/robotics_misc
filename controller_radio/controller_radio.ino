//Controls the robot. 2/23/18
/*
 * Control the robot. Connect radio usb to power, and DB9 cable
 * for data to lab computer. 
 * Open Putty. Set COM port appropriately (check in devices) 
 * and baud rate to 115200
 * Control via following 
 * 1 - 25% forward
 * 2 - 50% forward
 * 3 - 75% forward
 * 4 - 100% forward
 * 5 - 25% backwards
 * 6 - 50% backwards
 * 7 - 75% backwards
 * 8 - 100% backwards
 * 0 - Stop
 * 
 * This is not apparent in code because data is coming in corrupted.
 * To be fixed in future update
 *  
 */



//front left
const int dir_1_pin = 6;
const int m1_speed_pin = 7;
const int m1_sign = 1;

//front right
const int dir_2_pin = 9;
const int m2_speed_pin = 10;
const int m2_sign = 1;

//back left
const int dir_3_pin = 2;
const int m3_speed_pin = 3;
const int m3_sign = 1;

//back right
const int dir_4_pin = 11;
const int m4_speed_pin = 12;
const int m4_sign = 1;

const int COUNTER_MAX = 250;

int target_speed_1 = 0;
int actual_speed_1 = 0;

int target_speed_2 = 0;
int actual_speed_2 = 0;

int target_speed_3 = 0;
int actual_speed_3 = 0;

int target_speed_4 = 0;
int actual_speed_4 = 0;

int counter = 0;

#define HEADER     0

#define FRONT_LEFT 1
#define FRONT_RIGHT 2

#define MID_LEFT 3
#define MID_RIGHT 4

#define BACK_LEFT 5
#define BACK_RIGHT 6

#define CHECKSUM 7

char input_data[8];
int index = 0;
unsigned long lastTime = 0;
unsigned long currentTime = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial2.begin(9600);
  Serial3.begin(115200); //direct line to radio

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(dir_1_pin, OUTPUT);
  pinMode(m1_speed_pin, OUTPUT);
  pinMode(dir_2_pin, OUTPUT);
  pinMode(m2_speed_pin, OUTPUT);
  pinMode(dir_3_pin, OUTPUT);
  pinMode(m3_speed_pin, OUTPUT);
  pinMode(dir_4_pin, OUTPUT);
  pinMode(m4_speed_pin, OUTPUT);
}

// if no data has been received after three seconds, stop everything (soft kill)
void read_radio() {
    // put your main code here, to run repeatedly:
    currentTime = millis();
  if(Serial3.available()) {
    lastTime = currentTime;
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(LED_BUILTIN, HIGH);
    int data = Serial3.read();
    digitalWrite(LED_BUILTIN, LOW);
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
    
  } else {
    if ((currentTime - lastTime) >= 3000) {
      lastTime = currentTime;
      digitalWrite(LED_BUILTIN, HIGH);
      target_speed_1 = 0;
      target_speed_2 = 0;
      target_speed_3 = 0;
      target_speed_4 = 0;
      Serial.println("Timeout");
    }
  }

  if(index == 8) {

  /*
    for (int i = 0; i < 8; i++) {
      Serial.print((int) input_data[i]);
      Serial.print(" ");
    }
    Serial.println();
*/
    char sum = 0;
    for(int i = 1; i < 7; i++) {
      sum+= input_data[i];
    }
    sum += 0xAA;
    if(sum == input_data[CHECKSUM]) {
      for(int i = 0; i < 8; i++) {
        //Serial.print((int)input_data[i]);
        //Serial.print(", ");
      }
      //Serial.println();
      //checksum passed, set fields
      target_speed_1 = input_data[FRONT_LEFT] * 2;
      target_speed_2 = input_data[FRONT_RIGHT] * 2;
      target_speed_3 = input_data[BACK_LEFT] * 2;
      target_speed_4 = input_data[BACK_RIGHT] * 2;
      
    } else {
      //Serial.println("checksum fail");
    }
    index = 0;
  }
}

void loop() {
  read_radio();

//    // Do nothing we are at the right speed
//    if(target_speed == actual_speed) {
//     // Serial.println("reached target");
//      return;
//    }

    // Stall using counter as timer
    if(counter < COUNTER_MAX) {
      counter++;
    }

    // Adjust speed when counter is maxed, reset counter
    if(counter % COUNTER_MAX == COUNTER_MAX-1) {
      counter = 0;
      if(actual_speed_1 != target_speed_1) {
        actual_speed_1 < target_speed_1 ? actual_speed_1++: actual_speed_1--;
      }
      if(actual_speed_2 != target_speed_2) {
        actual_speed_2 < target_speed_2 ? actual_speed_2++: actual_speed_2--;
      }
      if(actual_speed_3 != target_speed_3) {
        actual_speed_3 < target_speed_3 ? actual_speed_3++: actual_speed_3--;
      }
      if(actual_speed_4 != target_speed_4) {
        actual_speed_4 < target_speed_4 ? actual_speed_4++: actual_speed_4--;
      }
      
    } else {
      return; //dont waste sending same val as before
    }

  Serial.print("Front Left:");
  set_pwm(dir_1_pin, m1_speed_pin, m1_sign, actual_speed_1);
  Serial.print(" Front Right:");
  set_pwm(dir_2_pin, m2_speed_pin, m2_sign, actual_speed_2);
  Serial.print(" Back Left:");
  set_pwm(dir_3_pin, m3_speed_pin, m3_sign, actual_speed_3);
  Serial.print(" Back Right:");
  set_pwm(dir_4_pin, m4_speed_pin, m4_sign, actual_speed_4);
  Serial.println();
   
    
}

void set_pwm(int dir_pin, int m_speed_pin, int sign, int m_actual_speed) {
  if((m_actual_speed * sign) >= 0) {
    digitalWrite(dir_pin, HIGH);
    Serial.print("+");
    analogWrite(m_speed_pin, m_actual_speed*sign);
    Serial.print(m_actual_speed*sign);
  } else {
    Serial.print("-");
    digitalWrite(dir_pin, LOW);
    analogWrite(m_speed_pin, -1*m_actual_speed*sign);
    Serial.print(-1*m_actual_speed*sign);
  }
}

