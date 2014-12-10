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
#include "sensor_mast.h"
#include "vfh.h"

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
   * @brief   Takes angle of reading and distance reading to stuff into a vector
   * @param   angle in radians, int distance inches
   * @return  range measure data type with angle with 90 degree heading and distance
   */
   VFH::range_measure assemble(int angle, int distance);
   
  private:
   
   
  
};

#endif
