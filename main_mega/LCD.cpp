/**
 ********************************************************************************************************
 * @file    lcd.cpp
 * @brief   lcd class
 * @details Controls LCD with certain parameters
 ********************************************************************************************************
 */
/*** INCLUDE FILES ***/

#include "Arduino.h"
#include "lcd.h"
#include <LiquidCrystal.h>


// this creates the lcd on the pins already on our board.
LiquidCrystal lcd(40, 41, 42, 43, 44, 45);

LCD::LCD() {
  // initialize
}

void LCD::setupLCD() {
  lcd.begin(16, 2);
  printLocationNow(0, 0, 0);
}

void LCD::service() {
  if (shouldRefresh) {
    refresh();
    // write out the current position
  }
}

void LCD::printLocationNow(float x, float y, float dir) {
  updateLocation(x, y, dir);
  refresh();
}

void LCD::updateLocation(float x, float y, float dir) {
  xPos = x;
  yPos = y;
  angle = dir;
}

void LCD::refresh() {
  lcd.setCursor(0, 1);
  char tempString[8];
  sprintf(tempString, "%02f:%02f:%02f", xPos, yPos, angle);
  lcd.print(tempString);
}
