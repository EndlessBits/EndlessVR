/*
  EndlessVR - DIY 1-bit VR Kit.

  Testing HX1230 screen, 96x68 pixels.
  The program renders a single pixel on display.

  Info get from https://www.instructables.com/HX1230-Monochrome-LCD-in-Arduino-Projects/

  ---------------------------------------------------------------
  Board:
  Arduino Uno
  ---------------------------------------------------------------

  ---------------------------------------------------------------
  Connection:
  ---------------------------------------------------------------
    GND to GND
    BL to 3.3V  /////or via resistor 10 Kohm to any digital pin
    VCC to 3.3V
    CLK to D13/SCK
    DIN to D11/MOSI
    N/C not connected
    CE to D7 or any digital
    RST to D6 or any digital

    "LCD works best at 3.3V, 5V is safe too but requires changes in contrast settings.
    The easiest way is to use Arduino Pro Mini powered from 3.3V source."

  ---------------------------------------------------------------
  Linraries:
  ---------------------------------------------------------------
  Install HX1230_SPI library to Arduino using HX1230_FB-master.zip file in VR_Screen_HX1230 folder.

  (There are 2 libraries, we use the second:
  1) Low resource library used for simple projects with mostly alfanumeric data
  (however rendering pixel graphics/bitmaps is still possible),
  doesn't use RAM for frame buffer, everything is rendered directly to LCD via SPI
    https://github.com/cbm80amiga/HX1230_SPI

  2) Full graphics library with dithering support: https://github.com/cbm80amiga/HX1230_FB

  Also you can search for "hx1230" in Arduino's Library Manager for more libraries.)

  ---------------------------------------------------------------
  Using two displays:
  ---------------------------------------------------------------
  Theoretically it's possible to connect both screens to a single Arduino"
  https://arduino.stackexchange.com/questions/8425/how-to-access-multiple-spi-interfaces-on-arduino

  But currently we use two boards.
*/


#define LCD_RST 6
#define LCD_CS  7
//#define LCD_DIN 11
//#define LCD_CLK 13

#include "HX1230_FB.h"
#include <SPI.h>
HX1230_FB lcd(LCD_RST, LCD_CS);


void setup()
{
  SPI.begin();
  lcd.init();
}

// --------------------------------------------------------------------------

void loop()
{
  lcd.cls();
  lcd.drawPixel(45,30,1); //left eye
  //lcd.drawPixel(95-45,30,1);   //right eye
  lcd.display();

  delay(15);
  //lcd.drawLine(0,0,30,20, 1);

}
