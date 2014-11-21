/**
 ********************************************************************************************************
 * @file    vex_controller.h
 * @brief   vex controller class
 * @details handles controller methods
 ********************************************************************************************************
 */

#ifndef vex_controller_h
#define vex_controller_h

#include "PPM.h"

class Controller {
  public:
    /**
   * @brief   Class constructor for controller
   * @param   a pin... if we could choose which Serial to use in the future
   * @return  Controller object
   */
    Controller(int pin);
   /**
   * @brief   returns the selected channel value
   * @param   channel number
   * @return  int value from the channel (something around 0-180)
   */
    int getControllerChannel(int channel);
   /**
   * @brief   checks if the up button is pressed on the passed channel
   * @param   channel number to check buttons on
   * @return  True if button is pressed, false if otherwise
   */
    bool isUpPressed(int channel);
   /**
   * @brief   checks if the down button is pressed on the passed channel
   * @param   channel number to check buttons on
   * @return  True if button is pressed, false if otherwise
   */
    bool isDownPressed(int channel);
  
  private:
    
};

#endif
