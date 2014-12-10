/**
 ********************************************************************************************************
 * @file    sonic_assembler.cpp
 * @brief   sonic assembler class
 * @details Assembles direction vectors (point cloud) from sensor mast
 ********************************************************************************************************
 */
/*** INCLUDE FILES ***/

#include "Arduino.h"
#include "sonic_assembler.h"

SonicAssembler::SonicAssembler() {
  // initialize
}

VFH::range_measure SonicAssembler::assemble(int angle, int distance) {
  VFH::range_measure data;
  data.dir = angle;
  data.distance = distance;
  return data;
}
