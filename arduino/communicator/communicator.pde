/*
  LiquidCrystal Library - Hello World
 
 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the 
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.
 
 This sketch prints "Hello World!" to the LCD
 and shows the time.
 
  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 
 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 25 July 2009
 by David A. Mellis
 
 
 http://www.arduino.cc/en/Tutorial/LiquidCrystal
 */

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 7, 6, 5, 4, 3);

int onOffPin = 9;
int blinkRate = 500;
int timeOn = 3000;
int lastBlink = 0;
int isOn = 0;
char currMsg[16];
char nextMsg[16];
int bReading = 0;
int bHaveNewMsg = 0;
int index = 0;
int anaPin = 0;

void setup() {
  // set up the LCD's number of rows and columns: 
  Serial.begin(57100);
  lcd.begin(16, 2);
  pinMode(onOffPin, OUTPUT);
  // Print a message to the LCD.
  //lcd.autoscroll();
  lcd.print("Don't Steal!");
  digitalWrite(onOffPin, HIGH);
  isOn = 1;
  memset(currMsg,0,16);
}

void loop() {
  //checkBlink();
  checkOnOff();
  checkSerial();
  if(bHaveNewMsg){
    writeNewMsg();
  }
  //int reader = analogRead(0);
  //Serial.print(reader/4);
  //Serial.print('\n');
}

void writeNewMsg(){
  lcd.clear();
  lcd.noCursor();
  lcd.print(currMsg); 
  lastBlink = millis();
  isOn = 1;
  bHaveNewMsg = 0;
  digitalWrite(onOffPin,HIGH);
}

void checkOnOff(){
  if(isOn){
    if((millis() - lastBlink)  > timeOn){
      digitalWrite(onOffPin, LOW);
      lcd.clear();
      isOn = 0;
    }
  }
}

void checkSerial(){
  if(Serial.available()){
    //Serial.print(Serial.read());
    if(bReading){
      nextMsg[index] = Serial.read();
      Serial.print(nextMsg[index]);
      if(nextMsg[index] == '\r'){
        memset(currMsg,0,16);
        strcpy(currMsg,nextMsg);
        memset(nextMsg,0,16);
        index = 0;
        bHaveNewMsg = 1;
        bReading = 0;
        Serial.print("Stopping Read.\n");
      } else {
        index = (index + 1) % 16;
      }
    } else {
      if(Serial.read() == 'b'){
         bReading = 1;
         Serial.print("Beginning to Read.\n");
      }
    }
  }
}
 
void checkBlink(){
  int now = millis();
  if (now - lastBlink > blinkRate){
    if(isOn){
      digitalWrite(onOffPin,LOW);
      isOn = 0;
      lastBlink = now;
    } else {
       digitalWrite(onOffPin,HIGH);
       isOn = 1;
       lastBlink = now;
    }
  }
}

