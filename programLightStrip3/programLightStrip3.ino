#include <Adafruit_NeoPixel.h>
//int sensorPin = A0;   // select the analog input pin for the photoresistor
//int sensorValue = 0;  // photoresistor value read from analog input pin
///////////////////////
#include <OneWire.h>
#include <SoftwareSerial.h>

int DS18S20_Pin = 4; //DS18S20 Signal pin on digital 4
char tmpstring[10];

//Temperature chip i/o
OneWire ds(DS18S20_Pin);  // on digital pin 4


#define PIN 6

int NUM_LEDS = 150;
int BRIGHTNESS= 70;
int nums=0;
int val=0;
int val2=0;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

///////////////////////////////////
//Product based on : https://www.amazon.com/ALITOVE-WS2812B-Individually-Addressable-Waterproof/dp/B00ZHB9M6A
//Code Written by Kevin Li
//Code tested on a photoresistor as a light sensor if temperature sensor is not available. 
//Library (Github): https://github.com/adafruit/Adafruit_NeoPixel
//Helpful Video: https://www.youtube.com/watch?v=bdIij70_VN0
///////////////////////////////////
///////////////////////////////////
void setup() {
  Serial.begin(9600);
  pixels.setBrightness(BRIGHTNESS);
    pixels.begin(); // This initializes the NeoPixel library.
    for(int i=0;i<=150;i=i+1){
          pixels.setPixelColor(i, pixels.Color(0,0,0)); 
    pixels.show(); // This sends the updated pixel color to the hardware.
    }
}
//////////////////////////////////////////////////
//////////////////////////////////////////////////
void loop() {
float temperature = getTemp();
  int tmp = (int) temperature;

  Serial.print(tmp);
  Serial.print("    ");  
  Serial.print(temperature);  
  delay(20); //just here to slow down the output so it is easier to read
 val=constrain(map(temperature*100, 7000,9000,0,255), 0, 255);
 ////temperature *100
//sensor();
   for(int i=0;i<=150;i=i+1){
      pixels.setPixelColor(i, pixels.Color(val,0,255-val));
          
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255 pixels.setPixelColor(i, pixels.Color(0,150,0)); // Moderately bright green color.
      }
      pixels.show(); // This sends the updated pixel color to the hardware.
   Serial.print("    ");  
Serial.println(val);


}
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
//void sensor(){
//  val = analogRead(0);
// val2=map(val, 60,100,0,100); ////converts a variable from a scaled range to another scaled range
////Serial.println(val);
//Serial.print(val);
//Serial.print("   ");
//Serial.println(val2);
//return val2;
//  }
//////////////////////////////////////
///Temperature
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
