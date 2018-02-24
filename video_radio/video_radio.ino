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
const int dir_4_pin = 12;
const int m4_speed_pin = 13;
const int m4_sign = 1;

const int COUNTER_MAX = 250;

int target_speed = 0;
int actual_speed = 0;

int counter = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial2.begin(9600);
  
  pinMode(dir_1_pin, OUTPUT);
  pinMode(m1_speed_pin, OUTPUT);
  pinMode(dir_2_pin, OUTPUT);
  pinMode(m2_speed_pin, OUTPUT);
  pinMode(dir_3_pin, OUTPUT);
  pinMode(m3_speed_pin, OUTPUT);
  pinMode(dir_4_pin, OUTPUT);
  pinMode(m4_speed_pin, OUTPUT);
}

void loop() {
  if(Serial2.available()) {
      byte reading = Serial2.read();
      Serial.print((unsigned int)reading);
      Serial.print(", ");
      Serial.println((char)reading);
      //Serial.print(",");
      //Serial.println(Serial2.available());

      switch(reading) 
      {
        case 'q': //1 forward 1/4 speed
          target_speed = 255/4;
          break;
        case 'r':
          target_speed = 255/2;
          break;
        case 's':
          target_speed = 3*255/4;
          break;
        case 't': //4 forward full speed
          target_speed = 255;
          break;
        case 'u': //5 backwards 1/4 speed
          target_speed = -255/4;
          break;
        case 'v': //6 
          target_speed = -255/2;
          break;
        case 'w': //7
          target_speed = -3*255/4;
          break;
        case 'x': //8 backwards full speed
          target_speed = -255;
          break;
        case 'p': //0 stop
          target_speed = 0;
          break;
        default:
          break;
          //target_speed = 0;
      }
    }


    // Do nothing we are at the right speed
    if(target_speed == actual_speed) {
     // Serial.println("reached target");
      return;
    }

    // Stall using counter as timer
    if(counter < COUNTER_MAX) {
      counter++;
    }

    // Adjust speed when counter is maxed, reset counter
    if(counter % COUNTER_MAX == COUNTER_MAX-1) {
      counter = 0;
      if(actual_speed < target_speed)
       actual_speed++;
      else
      actual_speed--;
    }

  set_pwm(dir_1_pin, m1_speed_pin, m1_sign);
  set_pwm(dir_2_pin, m2_speed_pin, m2_sign);
  set_pwm(dir_3_pin, m3_speed_pin, m3_sign);
  set_pwm(dir_4_pin, m4_speed_pin, m4_sign);
   
    
}

void set_pwm(int dir_pin, int m_speed_pin, int sign) {
  if((actual_speed * sign) >= 0) {
    digitalWrite(dir_pin, HIGH);
    analogWrite(m_speed_pin, actual_speed*sign);
  } else {
    digitalWrite(dir_pin, LOW);
    analogWrite(m_speed_pin, -1*actual_speed*sign);
  }
}

