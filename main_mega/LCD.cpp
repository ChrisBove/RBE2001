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
  // initialize nothing
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

void LCD::printLocationNow(float x, float y, float z) {
  updateLocation(x, y, z); // save location
  refresh(); // refresh display
}

void LCD::updateLocation(float x, float y, float z) {
  xPos = x;
  yPos = y;
  zPos = z;
}

void LCD::refresh() {
  // hacky way to do this, but it works.
  lcd.setCursor(0, 0);
  lcd.print("X:");
  lcd.print(xPos);
  lcd.setCursor(0,1);
  lcd.print("Y:");
  lcd.print(yPos);
  lcd.setCursor(9,1);
  lcd.print("Z:");
  lcd.print(zPos);
}
