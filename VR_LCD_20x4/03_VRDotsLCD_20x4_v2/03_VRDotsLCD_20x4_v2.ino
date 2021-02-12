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

LiquidCrystal lcd(2, 3, 4, 5, 6, 7); //rs, en, d4, d5, d6, d7);


//0 - empty
//1 - dot1
//2 - dot2
byte sprites[3][8] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 1},
  {0, 0, 0, 0, 0, 0, 0, 1}
};

//пиксель в строке
byte point_code[5] = {0b10000, 0b01000, 0b00100, 0b00010, 0b00001};

//--------------------------------------------------------
const byte symw = 5; //ширина символа
const byte symh = 8; //высота символа

const byte w = 100; //ширина всего дисплея
const byte w1 = w / 2; //ширина одного экрана
const byte h = 32; //высота одного экрана

byte symx[2] = {0, 0}; //символ последней точки для левого и правого экрана
byte symy[2] = {0, 0};

//--------------------------------------------------------
//печать символа в заданной позиции
//0 - empty
//1 - dot1
//2 - dot2
void print_sym(byte sx, byte sy, byte symbol) {
  if (sx < w && sy < h) {
    lcd.setCursor(sx, sy);
    //lcd.print(" "); //empty
    lcd.write(byte(symbol));
  }
}

//--------------------------------------------------------
//установка точки
//при этом, мы определяем, на каком экране - и заменяем символ точки
//x=0..20*5, 0..100     0..49 - левый экран, 50..99 - правый
//y=0..4*8, 0..32


void set_point(byte x, byte y) {
  byte s = (x < w1) ? 0 : 1; //0,1 - номер экрана
  byte spr = s + 1;  //1,2 - спрайт

  //символ
  byte sx = x / symw;
  byte sy = y / symh;

  //положение в символе
  byte posx = x % symw;
  byte posy = y % symh;

  //создаем символ
  byte *sprite = sprites[spr];
  for (byte i = 0; i < symh; i++) {
    sprite[i] = 0;
  }
  sprite[posy] = point_code[posx];
  lcd.createChar(spr, sprite);

  //затираем текущую точку на этом экране, если требуется
  if (sx != symx[s] || sy != symy[s]) {
    print_sym(symx[s], symy[s], 0);
  }
  symx[s] = sx;
  symy[s] = sy;
  print_sym(symx[s], symy[s], spr);

  //lcd.setCursor(5, 3);
  //lcd.print(s);

}

//--------------------------------------------------------

void setup() {
  // initialize LCD and set up the number of columns and rows:
  lcd.begin(20, 4);

  lcd.createChar(0, sprites[0]);

  set_point(0,0);
  
  //set_point(60,0);
  
/*  lcd.createChar(0, dot1);
  lcd.createChar(1, dot2);

  lcd.setCursor(3, 2);
  //lcd.print(" "); //empty
  lcd.write(byte(0)); //dot1

  lcd.setCursor(17, 2);
  //lcd.print(" "); //empty
  lcd.write(byte(1)); //dot2
*/
}

byte x1 = 12;
byte x2 = 14;

byte x = 0;
int dirx = 1;

byte y = h/2;

void loop() {
  set_point(x1 + x,y);
  set_point(w - x1 + x,y);
  x+=dirx;
  if (x <= 0) dirx = 1;
  if (x >= 3) dirx = -1;

  y++;
  y%=h;

  /*lcd.setCursor(0, 3);
  lcd.print("  ");
  lcd.setCursor(0, 3);
  lcd.print(x);
  */
  delay(2000);
}
