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
#include "WProgram.h"
void setup();
void loop();
void checkBlink();
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int onOffPin = 13;
int blinkRate = 500;
int lastBlink = 0;
int isOn = 0;
char currMsg[16];

void setup() {
  // set up the LCD's number of rows and columns: 
  lcd.begin(16, 2);
  pinMode(onOffPin, OUTPUT);
  // Print a message to the LCD.
  lcd.print("Get Set!");
  digitalWrite(onOffPin, HIGH);
  isOn = 1;
  memset(currMsg,0,16);
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  //checkBlink();
  Serial.begin(9600);
}
 
void checkBlink(){
  if(Serial.available()){
    memset(currMsg,0,16);
    char c = 1;
    int index = 0;
    while(c > 0){
      c = Serial.read();
      currMsg[index] = c;
    }
    lcd.print(currMsg);
  }
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


int main(void)
{
	init();

	setup();
    
	for (;;)
		loop();
        
	return 0;
}

