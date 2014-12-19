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
   * @param   angle in degrees, int distance inches
   * @return  range measure data type with angle with 90 degree heading and distance
   */
   VFH::range_measure assemble(int angle, int distance);
   
   // array functions
   
   /**
   * @brief   Empties out the measurement array
   * @param   None
   * @return  None
   */
   void clearArray();
   /**
   * @brief   Takes angle of reading and distance reading to stuff into a vecor stored in an array
   * @param   index to store, angle in radians, int distance inches
   * @return  range measure data type with angle with 90 degree heading and distance
   */
   void assembleInArray(int index, int angle, int distance);
   
   static const int arraySize = 19; // set size of array for measurements
   VFH::range_measure measure[arraySize]; // stores measurements for the grid
   
  private:
  
    int index = 0; // used to track position in array
   
  
};

#endif
