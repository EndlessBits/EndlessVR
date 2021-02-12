/*
VSS -   GND 
VDD – 5V       питание 
VO - контраст, к земле через потенциометр 5 КОм (либо два по 10 КОм, параллельно) (либо потенциометру) 
RS      - pin 2   регистр команд 
R/W          - GND, отправляем данные 
E enable   - пин 3   
D0..D7      -  подключаем только  D4,D5,D6,D7 к  пинам 4,5,6,7 
A led positive    5V      подсветка 
K led negative    GND 

 http://www.arduino.cc/en/Tutorial/LiquidCrystalHelloWorld

*/

#include <LiquidCrystal.h>

LiquidCrystal lcd(2,3,4,5,6,7); //rs, en, d4, d5, d6, d7);

void setup() {
  pinMode(9,OUTPUT);
  analogWrite(9,160);
  lcd.begin(20,4);
  lcd.print("hello, world!");
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(millis() / 1000);
}
