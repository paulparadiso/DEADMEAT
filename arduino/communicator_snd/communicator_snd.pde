#include <SoftwareSerial.h>

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
int angle = 10;

unsigned long startMillis;
unsigned int loops = 0;
unsigned int iter = 0;

int txPin = 5;
int rxPin = 4;

SoftwareSerial SpeakJetSerial =  SoftwareSerial(rxPin, txPin);

int wordsStart = 102;
int numWords = 12;
//20, 96, 21, 114, 22, 88, 23, 5,
char forward[] = {88, 5, 186, 7, 153, 7, 147, 151, 176,0}; 
char right[]   = {88, 148, 7, 155, 191,0};
char died[]    = {88, 14, 175, 8, 157, 174,0};
char stp[]     = {88,187, 191, 8, 136, 8, 199,0};
char walk[]    = {88, 5, 147, 8, 136, 8, 197,0};
char left[]    = {88, 5, 145, 131, 186, 191,0};
char turn[]    = {88, 8, 191, 151, 141,0};
char have[]    = {88, 5, 183, 8, 132, 166,0};
//char are[]     = {20, 96, 21, 114, 22, 88, 23, 5, 140, 8, 128, 191};
char are[]     = {88,8,152,0};
char dead[]    = {88, 23, 5, 174, 8, 131, 174,0};
char meat[]    = {88, 140,154, 191,0};
char you[]     = {88, 23, 9,183, 160,0};
char won[]     = {88,23, 8, 147,135, 142,0};

char walkForward[] = {88, 5, 147, 8, 136, 8, 197, 186, 7, 153, 7, 147, 151, 176,0};
char deadmeat[] = {88, 23, 9,183, 160, 8,152,174, 8, 131, 174,140,154, 191,0};
char youhavedied[] = {88, 23, 9,183, 160, 183, 8, 132, 166, 14, 175, 8, 157, 174,0};
char turnLeft[] = {88, 8, 191, 151, 141, 145, 131, 186, 191,0};
char turnRight[] = {88, 8, 191, 151, 141, 148, 7, 155, 191,0};
char youhavewon[] = {88, 23, 9,183, 160, 183, 8, 132, 166, 8, 147,135, 142,0};

//char *words[] = {
//  forward,
//  right,
//  died,
//  stp,
//  walk,
//  left,
//  turn,
//  have,
//  are,
//  dead,
//  meat,
//  you};f

/*
sayings:
"turn left" 
"turn right"
"walk forward"
"stop"
"you have died"
"you are dead meat"
*/

void setup(){
  Serial.begin(115200);
  
  //Setup pins for compass
  pinMode(EN_pin, OUTPUT);
  pinMode(CLK_pin, OUTPUT);
  pinMode(DIO_pin, INPUT);
  
  //Setup output pin and software serial for 
  pinMode(txPin, OUTPUT);
  SpeakJetSerial.begin(9600);

  HM55B_Reset();
  
  SoftwareSerial SpeakJetSerial =  SoftwareSerial(rxPin, txPin);
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
    }
    int o = (int)on;
    if((o - 87) >= stopPin && (o - 87) <= rightPin){
      turnOnPin(o);
    }
    if(on == 'e'){
      Serial.println(angle);
    }
    if(o >= wordsStart && o <= wordsStart + numWords){
      SpeakJetSerial.println("\0");  
      //SpeakJetSerial.println(youhave);
      switch(o){
        case 102:
          SpeakJetSerial.println(turnLeft);
          break;
        case 103:
          SpeakJetSerial.println(turnRight);
          break;
        case 104:
          SpeakJetSerial.println(walkForward);
          break;
         case 105:
          SpeakJetSerial.println(stp);
          break;
         case 106:
          SpeakJetSerial.println(deadmeat);
          break;
         case 107:
          SpeakJetSerial.println(youhavewon);
          break;
         case 108 :
          SpeakJetSerial.println(youhavedied);
          break;
         case 109:
          SpeakJetSerial.println(have);
          break;
         case 110:
          SpeakJetSerial.println(turn);
          break;
         case 111:
          SpeakJetSerial.println(dead);
          break;
         case 112:
          SpeakJetSerial.println(meat);
          break;
         case 113:
          SpeakJetSerial.println(you);
          break;
         case 114:
          SpeakJetSerial.println(died);
          break; 
         default:
           break;
      }
      //Serial.println(sizeof(words[o-wordsStart]));  
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
