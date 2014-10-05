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

LittleBrain brain(LittleBrain::WAIT_FOR_BUTTON);
Controller controller(2);
DriveTrain driveTrain(7, 6, 1, 0);

LineFollow follow(0, 1, 2, 3);
Button frontBumper(22);
BluetoothSlave btSlave;

// saddening globals
  int isInitialized = false;
  int weMadeIt = false;
  int result = 0;
  
  int crossingCount = 0;
  bool isBumped = false;

void setup() {
  driveTrain.attachMotors();
  driveTrain.halt();
  
  frontBumper.setupButton();
  
    // set up the timer (it starts automatically)
  Timer1.initialize(100000);	               // set up a 100 millisecond timer period
  Timer1.attachInterrupt(timer1ISR);           // ...and specify the timer ISR
  btSlave.setupBluetooth();
  
}

void loop() {
  // do updates
  btSlave.update();
  isBumped = frontBumper.isBumped();
  // STOP! If we hit the button, switch back to teleop
  if(!((controller.getControllerChannel(6)) > 130 || (controller.getControllerChannel(6) < 50)) )
    brain.thoughtState = LittleBrain::TELEOP;
  
  // enter state machine
  switch(brain.thoughtState) {
    case LittleBrain::WAIT_FOR_BUTTON:
      btSlave.goTime();
      brain.thoughtState = LittleBrain::TELEOP;
      break;
    case LittleBrain::TELEOP:
      reinitialize();
      driveTrain.moveMotors(controller.getControllerChannel(3), controller.getControllerChannel(2) );
      
      if(((controller.getControllerChannel(5)) > 130 || (controller.getControllerChannel(5) < 50)) )
      {
        brain.thoughtState = LittleBrain::CHOOSE_STORAGE_RACK;
      }
      // TODO check other buttons to determine next step
      break;
    
    case LittleBrain::CHOOSE_STORAGE_RACK:
      
      // assume we're starting at 1 and 1 side of the field
      btSlave.updateArrays();
      
      crossingCount = 0;
      for (int i = 0; i < 4; i++) {
        if (btSlave.storageArray[i] == 0)
          crossingCount = i + 1; // find closest open storage tube
      }
      if (crossingCount == 0) {
        Serial.println("We couldn't find an open tube!");
        brain.thoughtState = LittleBrain::TELEOP;
        break;
      }
      brain.thoughtState = LittleBrain::LINE_FOLLOW_CROSSING;
      break;
      
    case LittleBrain::LINE_FOLLOW_CROSSING:
      if (!weMadeIt)
        result = follow.stopOnCrossing(driveTrain, crossingCount);
      else {
        brain.thoughtState = LittleBrain::INIT_TURN;
      }
      if (result == 0)
        weMadeIt = false; // wait until we make it

      break;
      
    case LittleBrain::INIT_TURN:
      driveTrain.setTime();
      brain.thoughtState = LittleBrain::TURN;
      
    case LittleBrain::TURN:
      result = driveTrain.turn45(false); // turn right
      
      if (result == 1)
        brain.thoughtState = LittleBrain::TURN;
      break;
    
    case LittleBrain::LINE_FOLLOW_TO_PEG:
      if (!isBumped)
        follow.doLineFollow(driveTrain);
      else {
        driveTrain.halt();
        brain.thoughtState = LittleBrain::TELEOP;
      }
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
