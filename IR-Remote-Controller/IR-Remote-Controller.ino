#include <IRremote.h>
IRrecv IR(9);
decode_results cmd; // receive IR signal value as cmd

int speedPin = 6;
int dir1 = 4;
int dir2 = 5;
int mSpeed = 0;
bool powerOn = false;

#include <LiquidCrystal.h>
int rs = 7;
int en = 8;
int d4 = 10;
int d5 = 11;
int d6 = 12;
int d7 = 13;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#include <DHT.h>
#define Type DHT11
int sensePin = 2;
DHT HT (sensePin, Type);
float tempC;

void setup() {
  Serial.begin(9600);
  IR.enableIRIn();
  lcd.begin(16,2);
  HT.begin();
  pinMode(speedPin, OUTPUT);
  pinMode(dir1, OUTPUT);
  pinMode(dir2, OUTPUT);
}

void loop() {
  if (!powerOn) { // display current temp if not already using the fan 
    tempC = HT.readTemperature();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp:");
    lcd.print(tempC);
    lcd.print((char)223); // degree symbol
    lcd.print("C");


    if (tempC >= 20.0) {
      lcd.setCursor(0, 1);
      lcd.print("press power btn"); //suggest to user to press pwr btn to turn on fan
    }
  }
  
  digitalWrite(dir1, HIGH);
  digitalWrite(dir2, LOW);
  analogWrite(speedPin, mSpeed);

  // check for IR remote input
  if (IR.decode(&cmd)) {
    if (cmd.value == 0XFFA25D) {  // power btn
      mSpeed = 255;
      powerOn = true;  // display options for fan speed on screen
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("vol+ to up fan");
      lcd.setCursor(0, 1);
      lcd.print("vol- to slow fan");
    }
    if (cmd.value == 0XFFE21D) {  // power off btn
      mSpeed = 0;
      powerOn = false;  // go back to main menu and display current temp
      lcd.clear();
      lcd.print("Fan is OFF");
      delay(5000);
    }
    if (cmd.value == 0XFF629D){ // vol+ btn
      if (mSpeed < 255){
        mSpeed+= 5;
      }
      powerOn = true; // display current fan speed and option to decrease speed
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Fan Speed:");
      lcd.print(mSpeed);
      lcd.setCursor(0, 1);
      lcd.print("vol- to slow fan");
    }
    if (cmd.value == 0XFFA857){ // vol- btn
      if (mSpeed > 0){
        mSpeed-= 5;
      }
      powerOn = true; // display current fan speed and option to increase speed
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Fan Speed:");
      lcd.print(mSpeed);
      lcd.setCursor(0, 1);
      lcd.print("vol+ to up fan");
    }
    IR.resume();
  }

  delay(1000);
}
