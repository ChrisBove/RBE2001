// ***** INCLUDED LIBRARIEIS *******

// put them here...
#include <Servo.h>

#include <PPM.h>

#include <BluetoothClient.h>
#include <BluetoothMaster.h>
#include <ReactorProtocol.h>
#include <TimerOne.h>

#include "drive_train.h"
#include "vex_controller.h"
#include "line_follow.h"
#include "button.h"
#include "bluetooth_slave.h"
#include "little_brain.h"

LittleBrain brain(LittleBrain::TELEOP);
Controller controller(2);
DriveTrain driveTrain(7, 6, 1, 0);

LineFollow follow(0, 1, 2, 3);
Button frontBumper(22);
Button stopBumper(23);
BluetoothSlave btSlave;

// saddening globals
  int isInitialized = false;
  int weMadeIt = false;
  int result = 0;
  int reactorNum = 1;
  
  int crossingCount = 0;
  bool isBumped = false;
  bool stopBumped = false;
  bool stopChanged = false;
  bool isFirstBoot = true;

void setup() {
  driveTrain.attachMotors();
  driveTrain.halt();
  
  frontBumper.setupButton();
  stopBumper.setupButton();
  
    // set up the timer (it starts automatically)
  Timer1.initialize(100000);	               // set up a 100 millisecond timer period
  Timer1.attachInterrupt(timer1ISR);           // ...and specify the timer ISR
  btSlave.setupBluetooth();
//  Serial.begin(115200);
}

void loop() {
  // do updates
  btSlave.update();
  isBumped = frontBumper.isBumped();
  stopBumped = stopBumper.isBumped();
  // TODO - check for halt messages from field
  
  // -- respond to buttons ---
  // Stop button stuff, if hit for the first time, start things
  if (isFirstBoot && stopBumped) {
    btSlave.goTime();
    brain.thoughtState = LittleBrain::TELEOP;
    stopChanged = true;
    isFirstBoot = false;
  }
  // otherwise, this is an e-stop
  else if (!isFirstBoot && stopBumped && stopChanged) {
    isFirstBoot = true;
    stopBumped = false;
    stopChanged = false;
  }
//  else
//    if(stopBumped && !stopChanged) {
//      driveTrain.halt();
//      isFirstBoot = true;
//      stopChanged = false;
//    }
  if (!isFirstBoot && controller.isUpPressed(6)) {
    brain.thoughtState = LittleBrain::TELEOP;
  }
  // enter state machine
  if(!isFirstBoot)
  switch(brain.thoughtState) {      
      
    case LittleBrain::TELEOP:
      reinitialize();
      driveTrain.moveMotors(controller.getControllerChannel(3), controller.getControllerChannel(2) );
      
      if(controller.isUpPressed(5))
      {
        brain.thoughtState = LittleBrain::GRAB;
      }
      else if (controller.isDownPressed(5))
        brain.thoughtState = LittleBrain::REVERSE_FROM_SUPPLY;
      // TODO check other buttons to determine next step
      break;
    
    
    case LittleBrain::GRAB:
      
      brain.thoughtState = LittleBrain::EXTRACT;
      break;
      
      
    case LittleBrain::EXTRACT:
      
      follow.resetCrossCount();
      brain.thoughtState = LittleBrain::BACKUP;
      break;
      
      
    case LittleBrain::BACKUP:
      result = follow.stopOnCrossing(driveTrain, 1, DriveTrain::BACKWARD);

      if (result == 1)
        brain.thoughtState = LittleBrain::INIT_180;
      break;
      
      
    case LittleBrain::INIT_180:
      driveTrain.setTime();
      brain.thoughtState = LittleBrain::TURN_AROUND;
      break;
      
      
    case LittleBrain::TURN_AROUND:
      result = driveTrain.turn180(false);  // do a right turn
      if (result)
        brain.thoughtState = LittleBrain::CHOOSE_STORAGE_RACK;
      break;
      
      
    case LittleBrain::CHOOSE_STORAGE_RACK:
      // assume we're starting at 1 and 1 side of the field
      btSlave.updateArrays();
      crossingCount = 0;
      for (int i = 0; i < 4; i++) {
        if (btSlave.storageArray[i] == 0){
          crossingCount = i + 1; // find closest open storage tube
          break;
        }
      }
      follow.resetCrossCount();
      if (crossingCount == 0) {
        Serial.println("We couldn't find an open tube!");
        brain.thoughtState = LittleBrain::TELEOP;
        break;
      }
      brain.thoughtState = LittleBrain::LINE_FOLLOW_CROSSING;
      break;
      
      
    case LittleBrain::LINE_FOLLOW_CROSSING:
      result = follow.stopOnCrossing(driveTrain, crossingCount, DriveTrain::FORWARD);

      if (result == 1)
        brain.thoughtState = LittleBrain::INIT_TURN;

      break;
      
      
    case LittleBrain::INIT_TURN:
      driveTrain.setTime();
      brain.thoughtState = LittleBrain::TURN;
      break;
      
      
    case LittleBrain::TURN:
      if (reactorNum == 1)
        result = driveTrain.turn45(false); // turn left
      else
        result = driveTrain.turn45(true);
      
      if (result)
        brain.thoughtState = LittleBrain::LINE_FOLLOW_TO_PEG;
      break;
    
    
    case LittleBrain::LINE_FOLLOW_TO_PEG:
      if (!isBumped)
        follow.doLineFollow(driveTrain, DriveTrain::FORWARD);
      else {
        driveTrain.halt();
        brain.thoughtState = LittleBrain::TELEOP;
      }
      break;
      
    case LittleBrain::REVERSE_FROM_SUPPLY:
      result = follow.stopOnCrossing(driveTrain, 2, DriveTrain::BACKWARD);

      if (result == 1)
        brain.thoughtState = LittleBrain::TELEOP;
      break;
      
      
    default:
      brain.thoughtState = LittleBrain::TELEOP;
      break;
  // in teleop, just do teleop until a button is pressed to do something
  
  // in each other case, do something else
  }
  // drive to case - drive to 

//    if(!((controller.getControllerChannel(5)) > 130 || (controller.getControllerChannel(5) < 50)) ) {
//      driveTrain.moveMotors(controller.getControllerChannel(3), controller.getControllerChannel(2) );
//      follow.resetCrossCount();
//      isInitialized = false;
//      weMadeIt = false;
//      result = 0;
//    }
//    else
//    {
////      if (!frontBumper.isBumped())
////      follow.doLineFollowTillCross(driveTrain);
////      else driveTrain.halt();
//      if (!weMadeIt)
//        result = follow.stopOnCrossing(driveTrain, 3);
//      
//      if (result == 0)
//        weMadeIt = false; // wait until we make it
//      else {
//        // we made it!
//        weMadeIt = true;
//        if(!isInitialized) {
//          driveTrain.setTime();
//          isInitialized = true;
//        }
//        driveTrain.turn45(true); // turn right
//    }
    
//    if(!((controller.getControllerChannel(6)) > 130 || (controller.getControllerChannel(6) < 50)) ) {
//      follow.resetCrossCount();
//    }
//    else
//    {
//      if (follow.stopOnCrossing(driveTrain, 3) == 0)
//        ; // wait until we make it
//      else {
//        // we made it! YAAAAAAY!
//        
//      }
//    }
}

/*
 * ISR for timer1
 */
void timer1ISR() {
  btSlave.doTimerInterrupt();
}

void reinitialize() {
      follow.resetCrossCount();
      isInitialized = false;
      weMadeIt = false;
      result = 0;
}
