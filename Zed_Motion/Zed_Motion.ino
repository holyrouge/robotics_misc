#include <Servo.h>

void turnservo();

Servo myservo;  // create servo object to control a servo

int maxpos = 180;
int potpin = 0;  // analog pin used to connect the potentiometer
int val;    // variable to read the value from the analog pin

int pos = 0;    // variable to store the servo position

// constants won't change. They're used here to set pin numbers:
const int buttonPin = 52;     // the number of the pushbutton pin
const int ledPin =  9;      // the number of the LED pin

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

void setup() {
 myservo.attach(9);  // attaches the servo on pin 9 to the servo object
 Serial.begin(9600);
 // initialize the LED pin as an output:
// pinMode(ledPin, OUTPUT);
 // initialize the pushbutton pin as an input:
 pinMode(buttonPin, INPUT);
}

void loop() {
 // read the state of the pushbutton value:
 buttonState = digitalRead(buttonPin);

 // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
 if (buttonState == LOW) {
  // Do nothing
}
 
 if (buttonState == HIGH) {
   // turn LED on:
   turnservo();
 } 
 else {
  
 }
}

void turnservo() {
  val = analogRead(potpin);            // reads the value of the potentiometer (value between 0 and 1023)
  Serial.flush();
  Serial.println(val);
 val = map(val, 0, 1023, 0, 30);     // scale it to use it with the servo (value between 0 and 180)
 Serial.println(val);
for (pos = 0; pos <= maxpos; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(val);                       // waits 15ms for the servo to reach the position
  }
  for (pos = maxpos; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(val);                       // waits 15ms for the servo to reach the position
  }
}

