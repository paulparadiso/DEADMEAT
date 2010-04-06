#include <math.h>
#include <ks0108.h>
#include "Arial14.h"         // proportional font
#include "SystemFont5x7.h"   // system font
#include "rotateRight.h"     // bitmap 
#include "rotateLeft.h"
#include "walkForward.h"
#include "deadmeat.h"
#include "winner.h"
#include "getSet.h"
#include "jump.h"
#include "go.h"
#include "punch.h"
#include "kick.h"
#include "block.h"

int stopPin = 10;
int leftPin = 11;
int walkPin = 12;
int rightPin = 13;
int timeOn = 3000;
long lastOn = 0;
int onPin;
int bIsOn = 0;

byte CLK_pin = 2;
byte EN_pin = 3;
byte DIO_pin = 12;

int X_Data = 0;
int Y_Data = 0;
int angle;

//from glcd example
unsigned long startMillis;
unsigned int loops = 0;
unsigned int iter = 0;

uint8_t* sayings[] = {
  getSet,
  go,
  deadmeat,
  winner,
  jump,
  rotateLeft,
  rotateRight,
  walkForward,
  punch,
  kick,
  block};

int sayingsStart = 102;
int numSayings = 11;

void setup(){
  Serial.begin(115200);
  
  pinMode(EN_pin, OUTPUT);
  pinMode(CLK_pin, OUTPUT);
  pinMode(DIO_pin, INPUT);

  HM55B_Reset();
  
  GLCD.Init(NON_INVERTED);   // initialise the library, non inverted writes pixels onto a clear screen
  GLCD.ClearScreen();  
  GLCD.DrawBitmap(getSet, 0,0, BLACK); //draw the bitmap at the given x,y position
  GLCD.SelectFont(System5x7);
}

void loop(){
  checkSerial();
  checkOnOff();
  HM55B_StartMeasurementCommand(); // necessary!!
  delay(40); // the data is 40ms later ready
  HM55B_ReadCommand();  
  X_Data = ShiftIn(11); // Field strength in X
  Y_Data = ShiftIn(11); // and Y direction
  digitalWrite(EN_pin, HIGH); // ok deselect chip
  angle = 180 * (atan2(-1 * Y_Data , X_Data) / M_PI); // angle is atan( -y/x) !!!
}

void checkSerial(){
  if(Serial.available()){
    unsigned char on = Serial.read();
    if(on=='0'){
      GLCD.ClearScreen();
    }
    int o = (int)on;
    if((o - 87) >= stopPin && (o - 87) <= rightPin){
      turnOnPin(o);
    }
    if(on == 'e'){
      Serial.println(angle);
    }
    if(o >= sayingsStart && o <= sayingsStart + numSayings){
      GLCD.ClearScreen();
      GLCD.DrawBitmap(sayings[o - sayingsStart],0,0,BLACK);
    }
    //Serial.print(on);
  }
}

void turnOnPin(int _on){
  digitalWrite(onPin, LOW);
  digitalWrite(_on, HIGH);
  onPin = _on;
  lastOn = millis();
  bIsOn = 1;
}

void checkOnOff(){
  if(bIsOn){
    if(millis() - lastOn > timeOn){
      digitalWrite(onPin, LOW);
      bIsOn = 0;
    }
  }
}

void ShiftOut(int Value, int BitsCount) {
  for(int i = BitsCount; i >= 0; i--) {
    digitalWrite(CLK_pin, LOW);
    if ((Value & 1 << i) == ( 1 << i)) {
      digitalWrite(DIO_pin, HIGH);
      //Serial.print("1");
    }
    else {
      digitalWrite(DIO_pin, LOW);
      //Serial.print("0");
    }
    digitalWrite(CLK_pin, HIGH);
    delayMicroseconds(1);
  }
//Serial.print(" ");
}

int ShiftIn(int BitsCount) {
  int ShiftIn_result;
    ShiftIn_result = 0;
    pinMode(DIO_pin, INPUT);
    for(int i = BitsCount; i >= 0; i--) {
      digitalWrite(CLK_pin, HIGH);
      delayMicroseconds(1);
      if (digitalRead(DIO_pin) == HIGH) {
        ShiftIn_result = (ShiftIn_result << 1) + 1; 
        //Serial.print("x");
      }
      else {
        ShiftIn_result = (ShiftIn_result << 1) + 0;
        //Serial.print("_");
      }
      digitalWrite(CLK_pin, LOW);
      delayMicroseconds(1);
    }
  //Serial.print(":");

// below is difficult to understand:
// if bit 11 is Set the value is negative
// the representation of negative values you
// have to add B11111000 in the upper Byte of
// the integer.
// see: http://en.wikipedia.org/wiki/Two%27s_complement
  if ((ShiftIn_result & 1 << 11) == 1 << 11) {
    ShiftIn_result = (B11111000 << 8) | ShiftIn_result; 
  }


  return ShiftIn_result;
}

void HM55B_Reset() {
  pinMode(DIO_pin, OUTPUT);
  digitalWrite(EN_pin, LOW);
  ShiftOut(B0000, 3);
  digitalWrite(EN_pin, HIGH);
}

void HM55B_StartMeasurementCommand() {
  pinMode(DIO_pin, OUTPUT);
  digitalWrite(EN_pin, LOW);
  ShiftOut(B1000, 3);
  digitalWrite(EN_pin, HIGH);
}

int HM55B_ReadCommand() {
  int result = 0;
  pinMode(DIO_pin, OUTPUT);
  digitalWrite(EN_pin, LOW);
  ShiftOut(B1100, 3);
  result = ShiftIn(3);
  return result;
}
