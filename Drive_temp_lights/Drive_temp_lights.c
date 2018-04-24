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

#include <Adafruit_NeoPixel.h>
//int sensorPin = A0;   // select the analog input pin for the photoresistor
//int sensorValue = 0;  // photoresistor value read from analog input pin
///////////////////////
#include <OneWire.h>
#include <SoftwareSerial.h>


//Pin for temp sensor
int DS18S20_Pin = 4; //DS18S20 Signal pin on digital 4
char tmpstring[10];

//Temperature chip i/o
OneWire ds(DS18S20_Pin);  // on digital pin 4


//PIN for LEDs
#define PIN 6 


int NUM_LEDS = 150;
int BRIGHTNESS= 70;
int nums=0;
int val=0;
int val2=0;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);



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


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial2.begin(9600);
  Serial3.begin(115200); //direct line to radio
  
  pinMode(dir_1_pin, OUTPUT);
  pinMode(m1_speed_pin, OUTPUT);
  pinMode(dir_2_pin, OUTPUT);
  pinMode(m2_speed_pin, OUTPUT);
  pinMode(dir_3_pin, OUTPUT);
  pinMode(m3_speed_pin, OUTPUT);
  pinMode(dir_4_pin, OUTPUT);
  pinMode(m4_speed_pin, OUTPUT);

  //LEDs init
  pixels.setBrightness(BRIGHTNESS);
    pixels.begin(); // This initializes the NeoPixel library.
    for(int i=0;i<=150;i=i+1){
          pixels.setPixelColor(i, pixels.Color(0,0,0)); 
    pixels.show(); // This sends the updated pixel color to the hardware.
    }
}

void read_radio() {
    // put your main code here, to run repeatedly:
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

  if(index == 8) {

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

void update_leds() {
  float temperature = getTemp();
  int tmp = (int) temperature;

 val=constrain(map(temperature*100, 7000,9000,0,255), 0, 255);
 ////temperature *100
//sensor();
   for(int i=0;i<=150;i=i+1){
      pixels.setPixelColor(i, pixels.Color(val,0,255-val));    
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255 pixels.setPixelColor(i, pixels.Color(0,150,0)); // Moderately bright green color.
      }
    pixels.show(); // This sends the updated pixel color to the hardware.
}

void loop() {
  update_leds();
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


float getTemp(){
  //returns the temperature from one DS18S20 in DEG Celsius

  byte data[12];
  byte addr[8];

  if ( !ds.search(addr)) {
      //no more sensors on chain, reset search
      Serial.println("Can't find sensor, try a pullup resistor?");
      ds.reset_search();
      return -1000;
  }

  if ( OneWire::crc8( addr, 7) != addr[7]) {
      Serial.println("CRC is not valid!");
      return -1000;
  }

  if ( addr[0] != 0x10 && addr[0] != 0x28) {
      Serial.print("Device is not recognized");
      return -1000;
  }

  ds.reset();
  ds.select(addr);
  ds.write(0x44,1); // start conversion, with parasite power on at the end

  byte present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE); // Read Scratchpad

  for (int i = 0; i < 9; i++) { // we need 9 bytes
    data[i] = ds.read();
  }

  ds.reset_search();

  byte MSB = data[1];
  byte LSB = data[0];

  float tempRead = ((MSB << 8) | LSB); //using two's compliment
  float TemperatureSum = tempRead / 16;

  return (TemperatureSum * 18 + 5)/10 + 32;
}  
