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
  data.dir = angle; // * (180.0 *PI);
  data.distance = distance;
  return data;
}

void SonicAssembler::assembleInArray(int index, int angle, int distance) {
  measure[index].dir = angle; /* [degrees] */
  measure[index].distance = distance;
}

void SonicAssembler::clearArray() {
  for (int i = 0; i < arraySize; i ++) {
    measure[i].dir = 0;
    measure[i].distance = 0;
  }
}
