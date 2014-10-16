/*

  Description:
      This code is written for the RBE 2001 A14 Final Project Robot Wilber. 
    main_brain is the .ino file associated with the specific code for running this robot,
    and it contains the main state machine for the game.
    
      Teleop is a default state, one in which the robot starts and stops in before and
    after refueling reactors. Other states are set through a mostly linear progression.
    
    Pins are as defined below this comment block.
    
  Authors: Christopher Bove, Christopher Ellen, and Sami Neeno
  
  Date last modified: 10-16-2014
  
  NOTE:  This code requires use of Arduino 1.5.8 or higher in order to compile.

  Diagrams:
  
    North ^ is up
          |


        STO    1    2    3    4
               |    |    |    |
          -----+----+----+----+----
               |    |    |    |
          |    |    |    |    |    |
  REAC 1 -+----+----+----+----+----+- REAC 2
          |    |    |    |    |    |
               |    |    |    |
          -----+----+----+----+----
               |    |    |    |
        SUP    1    2    3    4


*/


// ***** INCLUDED LIBRARIEIS *******

// 3rd party libraries
#include <Servo.h>
#include <PPM.h>
// bluetooth and reactor libraries
#include <BluetoothClient.h>
#include <BluetoothMaster.h>
#include <ReactorProtocol.h>
// timer library for interrupts
#include <TimerOne.h>

// user-defined libraries, contained in root
#include "drive_train.h"
#include "vex_controller.h"
#include "line_follow.h"
#include "button.h"
#include "bluetooth_slave.h"
#include "little_brain.h"
#include "gripper.h"
#include "arm.h"

// ************* CONSTANTS AND PINS ***************
#define onBoardLEDPin   13
#define radiationLEDPin 30
#define reactorNumLED   31
#define controllerPin   2
#define leftMotorPin    7
#define rightMotorPin   6
#define gripServo       8
#define rackServo       9
#define leftSensor      0
#define middleSensor    1
#define rightSensor     2
#define rearSensor      3
#define frontSwitch     22
#define stopSwitch      23
#define frontArmLimit   24
#define armMotor        10
#define armPot          4

// *************** instantiate class objects **************
LittleBrain brain(LittleBrain::TELEOP);
Controller controller(controllerPin);
DriveTrain driveTrain(leftMotorPin, rightMotorPin, 1, 0); // left motor inverted, right not
Gripper gripper(gripServo, rackServo);
LineFollow follow(leftSensor, middleSensor, rightSensor, rearSensor);
Button frontBumper(frontSwitch);
Button stopBumper(stopSwitch);
Button frontLimit(frontArmLimit);
BluetoothSlave btSlave(onBoardLEDPin, radiationLEDPin); // change team number in here
Arm robotArm(armMotor, armPot);

// ****************** saddening globals **********************
int reactorNum = 1;        // number of the reactor we are on.
int crossingCount = 0;     // crossings to do
int winningIndex = 0;      // the index of the selected tube
bool isBumped = false;     // for the front bump switch, true if pressed
bool stopBumped = false;   // for the stop button, true if pressed
bool stopChanged = false;  // flag for servicing stop button presses
bool isFirstBoot = true;   // flag if this is the first boot
int doubleTapCount = 0;    // stored number of taps we've done
// autonomous globals
bool shouldTurnRight = false; // for storing whether to turn left or right
int winningSupplyIndex = 0;   // stores which index we chose from supply array

// ******************* SETUP *******************************
void setup() {
  driveTrain.attachMotors(); // attach motors in drivetrain
  driveTrain.halt();         // stop the drivetrain motors
  
  frontLimit.setupButton();    // setup the button on the arm forward limit
  frontBumper.setupButton();   // setup the front bumper stop
  stopBumper.setupButton();    // setup the stop button
  
  gripper.attachMotors();      // attach motors on gripper
  robotArm.setupArm();         // setup and attach motor on arm
  
  // configure reactorNumLED, set to represent reactor 1
  pinMode(reactorNumLED, OUTPUT);
  digitalWrite(reactorNumLED, HIGH);
  
  // this calibrates the line sensors. Make sure the center is on black, outers are on white!!!
//  follow.calibrate();

  // set up the timer (it starts automatically)
  Timer1.initialize(100000);	               // set up a 100 millisecond timer period
  Timer1.attachInterrupt(timer1ISR);           // ...and specify the timer ISR
  
  btSlave.setupBluetooth();  // setup the bluetooth slave
}

// ***** LOOP *****

void loop() {
  // do updates each loop! MUST BE RUN!!!
  btSlave.update();  // check for messages and send needed messages
  isBumped = frontBumper.isBumped();  // update bumper states
  stopBumped = stopBumper.isBumped();

  // check for halt messages from field
  if (btSlave.stopMovement)
    driveTrain.shouldMove = false;
  else if (btSlave.resumeMovement)
    driveTrain.shouldMove = true;

  // -- respond to buttons ---
  // Stop button stuff, if hit for the first time, start things
  if (isFirstBoot && stopBumped) {
    btSlave.goTime();  // start sending messages
    brain.thoughtState = LittleBrain::TELEOP; // set the state to TELEOP
    stopChanged = true;    // flag a changed stop button
    isFirstBoot = false;   // this is no longer the first boot
  }
  // otherwise, this is an e-stop, so reset stuff and stop stuff
  else if (!isFirstBoot && stopBumped && stopChanged) {
    isFirstBoot = true;
    stopBumped = false;
    stopChanged = false;
    driveTrain.halt();
  }
  // if the controller button is pressed at any time, revert to Teleop! (this is like an e-stop)
  if (!isFirstBoot && controller.isUpPressed(6)) {
    brain.thoughtState = LittleBrain::TELEOP;
  }

  // *********** enter state machine ****************

  /// NOTE: Each case breaks, allowing loop to execute every single time it can.
  // this allows the updates to take place at the beginning of each loop.
  // A case may be called multiple times until a condition is met that sets a
  // new case.
  int result = 0; // a local variable to store a result when needed (can't declare in a case)
  if (!isFirstBoot) // if this is not the first boot, do the main switch case
    switch (brain.thoughtState) {
        // switch based on stored state in brain
        // this is teleop, from here, the next autonomous action is taken
      case LittleBrain::TELEOP:
        reinitialize(); // reinitialize globals and turn off rad alerts (scroll way, way down)
        // update the drivetrain with input from the controller
        driveTrain.moveMotors(180- controller.getControllerChannel(3), controller.getControllerChannel(2) );
        // if the up button on channel 5 is pressed, switch to grab state
        if (controller.isUpPressed(5))
        {
          brain.thoughtState = LittleBrain::GRAB; // meant to grab rod FROM the first or second reactor
        }
        // otherwise, if the down is pressed on channel 5, start moving the gripper to grab the new rod.
        else if (controller.isDownPressed(5))
          brain.thoughtState = LittleBrain::GET_NEW_ROD;
        // otherwise, if the down is pressed on channel 5, start moving the gripper to grab the new rod.
        if(controller.isDownPressed(6)) {
          if (reactorNum == 1) {
            reactorNum = 2;
            digitalWrite(reactorNumLED, LOW);
          }
          // otherwise, go back to 1 in case we need to re do it
          else {
            reactorNum = 1;
            digitalWrite(reactorNumLED, HIGH);
          }
        }
          
          
        // TODO check other buttons to determine next step if needed
        break;

        // this grabs the rod in the reactor
        // ASSUMPTION: the robot is positioned along the line facing the reactor (East west line in between reactors).
        // reactor tube should be between fork
      case LittleBrain::GRAB:
        btSlave.setRadHigh(false);
        btSlave.setRadLow(true);

        if (gripper.closeTheGrip()) {
          brain.thoughtState = LittleBrain::EXTRACT; // next loop, do extraction
        }
        break;


        // this extracts the rod from the reactor, moving the fourbar to the vertical position after the rod is vert. moved.
      case LittleBrain::EXTRACT:
        if (gripper.retractTheGrip()) {
          
          brain.thoughtState = LittleBrain::EXTRACT_1;
        }
        break;
        
      case LittleBrain::EXTRACT_1:
        if (robotArm.goUp(frontLimit)) {
          follow.resetCrossCount(); // reset the cross count for our next state
          brain.thoughtState = LittleBrain::BACKUP;
        }
        break;

        // backs the robot up in BACKWARD line following until it hits the first cross
      case LittleBrain::BACKUP:
        // once we have hit the first cross, case is the next one
        if (follow.stopOnCrossing(driveTrain, 1, DriveTrain::BACKWARD))
          brain.thoughtState = LittleBrain::INIT_180;
        break;
        
      case LittleBrain::BACKUP_1:
        if (driveTrain.backupABit())
          brain.thoughtState = LittleBrain::INIT_180;
        break;


        // initiates a 180 degree turn by setting the drivetrain clock timer.
      case LittleBrain::INIT_180:
        driveTrain.setTime(); // set the clock to this time
        brain.thoughtState = LittleBrain::TURN_AROUND;
        break;


      case LittleBrain::TURN_AROUND:
        // do a right turn (specified by false)of 180 degrees
        // if the driveTrain returns done moving, then we're ready to go to the next case
        if (driveTrain.turn180(false))
          brain.thoughtState = LittleBrain::CHOOSE_STORAGE_RACK;
        break;

        // calculates the crossings to get to the closest empty storage rack.
      case LittleBrain::CHOOSE_STORAGE_RACK:
        // assume we're starting at 1 and 1 side of the field
        btSlave.updateArrays(); // update our arrays
        crossingCount = 1;      // reset crossing count
        // if reactor one, search array in one direction
        if (reactorNum == 1)
          for (int i = 0; i < 4; i++) {
            if (btSlave.storageArray[i] == 0) {
              crossingCount = i + 1; // find closest open storage tube
              winningIndex = i;
              break;
            }
          }
        else // else, search the array from a different direction
          for (int i = 0; i < 4; i++) {
            // TODO check this value
            if (btSlave.storageArray[i] == 0) {
              crossingCount = (i - 4) * -1; // find closest open storage tube
              winningIndex = i;
              break;
            }
          }

        follow.resetCrossCount();
//        if (crossingCount == 0) {
//          brain.thoughtState = LittleBrain::TELEOP; // revert to teleop
//          break;
//        }
        // done this case in one shot loop, so next loop do line crossing
        brain.thoughtState = LittleBrain::LINE_FOLLOW_CROSSING;
        break;

        // stop once we have reached the requested crossing count
      case LittleBrain::LINE_FOLLOW_CROSSING:
        // once we've made it to the specified crossing, set the next case
        if (follow.stopOnCrossing(driveTrain, crossingCount, DriveTrain::FORWARD))
          brain.thoughtState = LittleBrain::INIT_TURN;
        break;
        
      case LittleBrain::GO_A_BIT_FURTHER:
        if (driveTrain.forwardABit())
          brain.thoughtState = LittleBrain::INIT_TURN;
        break;
        


        // initiates a turn
      case LittleBrain::INIT_TURN:
        driveTrain.setTime();
        brain.thoughtState = LittleBrain::TURN;
        break;

        // does a 45 degree turn to face a storage rack (depends on which reactor we're traveling from)
      case LittleBrain::TURN:
        result = 0;
        if (reactorNum == 1)
          result = driveTrain.turn45(false); // turn left
        else
          result = driveTrain.turn45(true); // turn right
        // if done the turn, set to next state
        if (result)
          brain.thoughtState = LittleBrain::LINE_FOLLOW_TO_PEG;
        break;

        // do line following until we hit the storage peg
      case LittleBrain::LINE_FOLLOW_TO_PEG:
        follow.setForwardOnCross(true);
        if (!isBumped)
          follow.doLineFollow(driveTrain, DriveTrain::FORWARD);
        else {
          // when we hit it, stop!
          driveTrain.halt();
          brain.thoughtState = LittleBrain::INSERT_STORAGE; // next loop puts the rod in
        }
        break;
        

        // insert the rod into storage
      case LittleBrain::INSERT_STORAGE:
        if (gripper.extendTheGrip()) {
          brain.thoughtState = LittleBrain::CHECK_INSERTION; // next loop does teleop
        }
        break;
        
      case LittleBrain::CHECK_INSERTION:
        if (btSlave.isInStorage(winningIndex) || doubleTapCount > 4 )
          brain.thoughtState = LittleBrain::SET_FOR_NEW;
        else
          brain.thoughtState = LittleBrain::DOUBLE_TAP_0;
        
        break;

      case LittleBrain::DOUBLE_TAP_0:
        if(gripper.openTheGrip())
          brain.thoughtState = LittleBrain::DOUBLE_TAP_1;
        break;
      
      case LittleBrain::DOUBLE_TAP_1:
        if(gripper.retractTheGrip())
          brain.thoughtState = LittleBrain::DOUBLE_TAP_2;
        break;
        
      case LittleBrain::DOUBLE_TAP_2:
        if (gripper.closeTheGrip())
          brain.thoughtState = LittleBrain::DOUBLE_TAP_3;
        break;
        
      case LittleBrain::DOUBLE_TAP_3:
        if(gripper.extendLimTheGrip()) {
          brain.thoughtState = LittleBrain::DOUBLE_TAP_4;
        }
        break;
        
       case LittleBrain::DOUBLE_TAP_4:
        if(gripper.openTheGrip()) {
          doubleTapCount ++;
          brain.thoughtState = LittleBrain::CHECK_INSERTION;
        }
        break;

       case LittleBrain::SET_FOR_NEW:
        if(gripper.extendLimTheGrip()) {
//          brain.thoughtState = LittleBrain::TELEOP;
          brain.thoughtState = LittleBrain::A_REVERSE_FROM_STORAGE;
          btSlave.setRadLow(false);
          follow.resetCrossCount();
          doubleTapCount = 0;
        }
        break;
        
        
       // ==================== EXPERIMENTAL AUTONOMY FOR REFUELING =============================
       
       case LittleBrain::A_REVERSE_FROM_STORAGE:
        if (follow.stopOnCrossing(driveTrain, 1, DriveTrain::BACKWARD)) {
          brain.thoughtState = LittleBrain::A_PREP_REVERSE;
        }
         break;
       case LittleBrain::A_PREP_REVERSE:
         driveTrain.setTime();
         brain.thoughtState = LittleBrain::A_REVERSE;
         break;
       case LittleBrain::A_REVERSE:
         if (driveTrain.backupForTime()) {
           brain.thoughtState = LittleBrain::A_PREP_180;
         }
         break;
       case LittleBrain::A_PREP_180:
         driveTrain.setTime();
         brain.thoughtState = LittleBrain::A_DO_180;
         break;
       case LittleBrain::A_DO_180:
         // do a right turn
         if (driveTrain.turn180(false)) {
           brain.thoughtState = LittleBrain::A_CHOOSE_PATH;
           follow.resetCrossCount(); // reset for next linefollow
         }
         break;
        /*              STORAGE SIDE
            ARRAY  0    1    2    3
                   1    2    3    4
                   1    2    3    4
            ARRAY  0    1    2    3
                      SUPPLY SIDE
        */
       case LittleBrain::A_CHOOSE_PATH:
         btSlave.updateArrays();
         winningSupplyIndex = 0;
         for (int i = 0; i < 4; i++) {
            if (btSlave.supplyArray[i] == 1) {
              winningSupplyIndex = i;
              break;
            }
         }
         
         switch (winningSupplyIndex - winningIndex) {
           
           case 0:
             brain.thoughtState = LittleBrain::A_LINE_FOLLOW_TO_PEG;
             break;
           
           case -3 ... -1:
             // turn right
             shouldTurnRight = true;
             // count out crossings
             crossingCount = abs(winningSupplyIndex - winningIndex);
             brain.thoughtState = LittleBrain::A_GO_TO_CENTER;
             break;
           case 1 ... 3:
             // turn left
             shouldTurnRight = false;
             // count out crossings
             crossingCount = winningSupplyIndex - winningIndex;
             brain.thoughtState = LittleBrain::A_GO_TO_CENTER;
             break;
             
           default:
             brain.thoughtState = LittleBrain::A_LINE_FOLLOW_TO_PEG;
             break;           
         }
         follow.resetCrossCount();
         break;
         
       case LittleBrain::A_GO_TO_CENTER:
         if (follow.stopOnCrossing(driveTrain, 1, DriveTrain::FORWARD))
           brain.thoughtState = LittleBrain::A_INIT_TURN_TOWARDS_1;
         break;
       case LittleBrain::A_INIT_TURN_TOWARDS_1:
         driveTrain.setTime();
         brain.thoughtState = LittleBrain::A_TURN_TOWARDS_1;
         break;
       case LittleBrain::A_TURN_TOWARDS_1:
        if (driveTrain.turn45(shouldTurnRight))
          brain.thoughtState = LittleBrain::A_DRIVE_TO_NEXT_CROSSING;
        follow.resetCrossCount();
       break;
       case LittleBrain::A_DRIVE_TO_NEXT_CROSSING:
         if(follow.stopOnCrossing(driveTrain, crossingCount, DriveTrain::FORWARD))
           brain.thoughtState = LittleBrain::A_INIT_TURN_TO_SUPPLY;
         break;
       case LittleBrain::A_INIT_TURN_TO_SUPPLY:
         driveTrain.setTime();
         brain.thoughtState = LittleBrain::A_TURN_TO_SUPPLY;
         break;
       case LittleBrain::A_TURN_TO_SUPPLY:
         if (driveTrain.turn45(!shouldTurnRight))
           brain.thoughtState = LittleBrain::A_LINE_FOLLOW_TO_PEG;
         break;
       case LittleBrain::A_LINE_FOLLOW_TO_PEG:
         follow.setForwardOnCross(true);
         if (!isBumped)
           follow.doLineFollow(driveTrain, DriveTrain::FORWARD);
         else {
           driveTrain.halt();
           brain.thoughtState = LittleBrain::GET_NEW_ROD;
         }
         break;
       
       // ======================== END EXPERIMENTAL AUTONOMY ==================================

        // SEQUENCE FOR GETTING THINGS TO THE REACTOR
        // GET_NEW_ROD, REVERSE_FROM_SUPPLY, PREP_180, DO_180, GET_TO_CENTER
        // uses the gripper to get the new rod from the supply
      case LittleBrain::GET_NEW_ROD:
        if (gripper.closeTheGrip()) {
          btSlave.setRadLow(false);
          btSlave.setRadHigh(true);
          brain.thoughtState = LittleBrain::GET_NEW_ROD_1;
        }
        break;
        
        
      case LittleBrain::GET_NEW_ROD_1:
        if (gripper.retractTheGrip()) {
          follow.resetCrossCount();
          brain.thoughtState = LittleBrain::REVERSE_FROM_SUPPLY;
        }
        break;
        

        // backs away from the supply rack until hitting the little line in front of it
      case LittleBrain::REVERSE_FROM_SUPPLY:
        if (follow.stopOnCrossing(driveTrain, 1, DriveTrain::BACKWARD)) {
          brain.thoughtState = LittleBrain::INIT_REVERSE_AGAIN;
        }
        break;
        
        
      
      case LittleBrain::INIT_REVERSE_AGAIN:
        driveTrain.setTime();
        brain.thoughtState = LittleBrain::REVERSE_AGAIN;
        break;
        
      case LittleBrain::REVERSE_AGAIN:
        if (driveTrain.backupForTime()) {
          brain.thoughtState = LittleBrain::PREP_180;
        }
        break;
        

        // preps time for a 180
      case LittleBrain::PREP_180:
        driveTrain.setTime();
        brain.thoughtState = LittleBrain::DO_180;
        break;

        // does a 180 degree turn
      case LittleBrain::DO_180:
        // do a right turn
        if (driveTrain.turn180(false)) {
          brain.thoughtState = LittleBrain::GET_TO_CENTER;
          follow.resetCrossCount(); // reset for next linefollow
        }
        break;

        // linefollow until reaching center cross
      case LittleBrain::GET_TO_CENTER:
        // stop at first cross
        if (follow.stopOnCrossing(driveTrain, 1, DriveTrain::FORWARD))
          brain.thoughtState = LittleBrain::INIT_TURN_TO_REACTOR;
        break;
        
        
//      case LittleBrain::GET_TO_CENTER_1:
//        result = driveTrain.forwardABit();
//        
//        if (result)
//          brain.thoughtState = LittleBrain::INIT_TURN_TO_REACTOR;
//        break;


        // initialize turn towards the reactor
      case LittleBrain::INIT_TURN_TO_REACTOR:
        driveTrain.setTime();
        brain.thoughtState = LittleBrain::TURN_TO_REACTOR;
        break;

        // turn towards the reactor that we're refueling
      case LittleBrain::TURN_TO_REACTOR:
        result = 0;
        if (reactorNum == 1)
          result = driveTrain.turn45(false); // turn left
        else
          result = driveTrain.turn45(true); // turn right

        if (result)
          brain.thoughtState = LittleBrain::GET_TO_REACTOR;
        break;

        // line follow until we hit the reactor tube
      case LittleBrain::GET_TO_REACTOR:
        follow.setForwardOnCross(true);
        if (!isBumped)
          follow.doLineFollow(driveTrain, DriveTrain::FORWARD);
        else {
          driveTrain.halt();
          brain.thoughtState = LittleBrain::LOWER_ARM;
        }
        break;
        
      case LittleBrain::LOWER_ARM:
        if (robotArm.goDown(frontLimit)) {
          brain.thoughtState = LittleBrain::REFUEL_REACTOR_0;
        }
        break;

        // refuel the reactor using the gripper
        
      case LittleBrain::REFUEL_REACTOR_0:
        if (gripper.extendLimTheGrip())
          brain.thoughtState = LittleBrain::REFUEL_REACTOR_1;
        break;
        
      case LittleBrain::REFUEL_REACTOR_1:
        if (gripper.openTheGrip())
          brain.thoughtState = LittleBrain::REFUEL_REACTOR;
        break;
        
        
      case LittleBrain::REFUEL_REACTOR:
        // if we are on the first reactor, switch us to 2 for the next run
        if (reactorNum == 1) {
          reactorNum = 2;
          digitalWrite(reactorNumLED, LOW);
        }
        // otherwise, go back to 1 in case we need to re do it
        else {
          reactorNum = 1;
          digitalWrite(reactorNumLED, HIGH);
        }
        // when manip movements done, turn off radiation messages and go to teleop
        brain.thoughtState = LittleBrain::TELEOP;
        btSlave.setRadHigh(false);
        break;

        // this should never execute
      default:
        brain.thoughtState = LittleBrain::TELEOP;
        break;

    }
}
/*
 * ISR for timer1
 */
void timer1ISR() {
  btSlave.doTimerInterrupt();
}

// resets the globals that matter
void reinitialize() {
  follow.resetCrossCount();
}
