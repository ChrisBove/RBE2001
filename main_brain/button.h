/**
 ********************************************************************************************************
 * @file    button.h
 * @brief   button control class
 * @details Used to read buttons/switches
 ********************************************************************************************************
 */

#ifndef button_h
#define button_h

/*** INCLUDE FILES ***/

#include "Arduino.h"

class Button {
  public:
   /**
   * @brief   Class constructor for button.
   * @param   DIO pin of this button
   * @return  button object
   */
    Button(int pin);
   /**
   * @brief   attaches the button
   * @param   None
   * @return  None
   */
    void setupButton();
   /**
   * @brief   checks button state and returns if its pressed
   * @param   None
   * @return  true if pushed, false if otherwise
   */
    bool isBumped();
    
  private:
    int _pin; // pin of button
};

#endif
