/**
 ********************************************************************************************************
 * @file    sonic_assembler.h
 * @brief   sonic assembler class
 * @details Assembles direction vectors (point cloud) from sensor mast
 ********************************************************************************************************
 */

#ifndef sonic_assembler_h
#define sonic_assembler_h

#include "Arduino.h"

class SonicAssembler {
  public:
   /**
   * @brief   Class constructor for sonic assembler
   * @param   none
   * @return  sonic assembler object
   */
   SonicAssembler();
   /**
   * @brief   Runs all service routines
   * @param   None
   * @return  None
   */
   void service();
   /**
   * @brief   Runs all service routines
   * @param   angle in degrees (-90 to 90), int distance inches
   * @return  None
   */
   void assemble(int angle, int distance);
   
  private:
   
   
  
};

#endif
