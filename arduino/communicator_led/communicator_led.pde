#include <math.h>

int stopPin = 10;
int leftPin = 11;
int walkPin = 12;
int rightPin = 13;
int timeOn = 3000;
long lastOn = 0;
int onPin;
int bIsOn = 0;

byte CLK_pin = 7;
byte EN_pin = 8;
byte DIO_pin = 9;

int X_Data = 0;
int Y_Data = 0;
int angle;


void setup(){
  pinMode(stopPin,OUTPUT);
  pinMode(leftPin,OUTPUT);
  pinMode(walkPin,OUTPUT);
  pinMode(rightPin,OUTPUT);
  Serial.begin(9600);
  
  pinMode(EN_pin, OUTPUT);
  pinMode(CLK_pin, OUTPUT);
  pinMode(DIO_pin, INPUT);

  HM55B_Reset();
}

void loop(){
  checkSerial();
  checkOnOff();
  HM55B_StartMeasurementCommand(); // necessary!!
  delay(40); // the data is 40ms later ready
  //Serial.print(HM55B_ReadCommand()); // read data and print Status
  HM55B_ReadCommand();
  //Serial.print(" ");  
  X_Data = ShiftIn(11); // Field strength in X
  Y_Data = ShiftIn(11); // and Y direction
  //Serial.print(X_Data); // print X strength
  //Serial.print(" ");
  //Serial.print(Y_Data); // print Y strength
  //Serial.print(" ");
  digitalWrite(EN_pin, HIGH); // ok deselect chip
  angle = 180 * (atan2(-1 * Y_Data , X_Data) / M_PI); // angle is atan( -y/x) !!!
  //Serial.println(angle); // print angle
  //Serial.println("");

}

void checkSerial(){
  if(Serial.available()){
    unsigned char on = Serial.read();
    int o = ((int)on) - 87;
    if(o >= stopPin && o <= rightPin){
      turnOnPin(o);
    }
    if(on == 'e'){
      Serial.println(angle);
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
