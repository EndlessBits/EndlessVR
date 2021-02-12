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

 Based on Adafruit's example at
 https://github.com/adafruit/SPI_VFD/blob/master/examples/createChar/createChar.pde

 This example code is in the public domain.
 http://www.arduino.cc/en/Tutorial/LiquidCrystalCustomCharacter

 Also useful:
 http://icontexto.com/charactercreator/

*/


#include <LiquidCrystal.h>

LiquidCrystal lcd(2,3,4,5,6,7); //rs, en, d4, d5, d6, d7);


byte dot1[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b10000,
  0b00000,
  0b00000,
  0b00000
};

byte dot2[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00001,
  0b00000,
  0b00000,
  0b00000
};

void setup() {
  // initialize LCD and set up the number of columns and rows:
  lcd.begin(20, 4);

  lcd.createChar(0, dot1);
  lcd.createChar(1, dot2);

  lcd.setCursor(3, 2);
  //lcd.print(" "); //empty
  lcd.write(byte(0)); //dot1

  lcd.setCursor(17, 2);
  //lcd.print(" "); //empty
  lcd.write(byte(1)); //dot2

}

void loop() {
 
}
