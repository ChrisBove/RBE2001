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

// ************************** Supporting functions *************************
// ISR for timer1 
void timer1ISR() {
  btSlave.doTimerInterrupt();
}

// resets the lineFollower (in case of controller interrupt)
void reinitialize() {
  follow.resetCrossCount();
}

// sets up the drivetrain clock for the passed turn, puts brainState to that state
void setDriveTrain(LittleBrain::littleBrainState nextState) {
  driveTrain.setTime(); // set the clock to this time
  brain.thoughtState = nextState;
}

// **************************** LOOP **********************
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
        reinitialize(); // reinitialize globals
        // update the drivetrain with input from the controller
        driveTrain.moveMotors(180- controller.getControllerChannel(3), controller.getControllerChannel(2) );
        // if the up button on channel 5 is pressed, switch to grab state
        if (controller.isUpPressed(5)) {
          brain.thoughtState = LittleBrain::GRAB; // meant to grab rod FROM the first or second reactor
        }
        // otherwise, if the down is pressed on channel 5, start moving the gripper to grab the new rod.
        else if (controller.isDownPressed(5))
          brain.thoughtState = LittleBrain::GET_NEW_ROD;
        
        // change reactor number if down is pressed, update LED status
        if(controller.isDownPressed(6)) {
          if (reactorNum == 1) {
            reactorNum = 2;  // if we're on 1, go to 2
            digitalWrite(reactorNumLED, LOW); // off if on 2
          }
          // otherwise, go back to 1 in case we need to re do it
          else {
            reactorNum = 1;
            digitalWrite(reactorNumLED, HIGH); // on if on 1
          }
        }
        break;


        // this grabs the rod in the reactor
        // ASSUMPTION: the robot is positioned along the line facing the reactor (East west line in between reactors).
        // reactor tube should be between fork
      case LittleBrain::GRAB:
        // set new radiation status - rod is being exposed
        btSlave.setRadHigh(false);
        btSlave.setRadLow(true);
        // close the grip, if done, go to next state
        if (gripper.closeTheGrip()) {
          brain.thoughtState = LittleBrain::EXTRACT; // next loop, do extraction
        }
        break;


        // this extracts the rod from the reactor using the rack and pinion
      case LittleBrain::EXTRACT:
        if (gripper.retractTheGrip()) {
          brain.thoughtState = LittleBrain::EXTRACT_1;
        }
        break;
        
      // moves the fourbar to the vertical position after the rod is vert. moved.
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
          setDriveTrain(LittleBrain::TURN_AROUND);
        break;
        
        
      // do a 180 degree to turn to reverse direction
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
        // search array for open storage tubes
        for (int i = 0; i < 4; i++) {
          if (btSlave.storageArray[i] == 0) {
            if (reactorNum == 1)
              crossingCount = i + 1; // find closest open storage tube
            else
              crossingCount = (i - 4) * -1; // find closest open storage tube to other reactor
            winningIndex = i; // this is the index we chose
            break;
          }
        }
        
        follow.resetCrossCount(); // reset cross count for line following
        // uncomment next block and change crossingCount initialization to 0 to revert to tele if a tube is not found
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
          setDriveTrain(LittleBrain::TURN);
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
          brain.thoughtState = LittleBrain::RELEASE_ROD;
        }
        break;
        
        
      // open the grip
      case LittleBrain::RELEASE_ROD:
        if (gripper.openTheGrip())
          brain.thoughtState = LittleBrain::CHECK_INSERTION;
        break;
        
      
      // check array to see if our selected storage rack has now been filled, only try 5 times
      case LittleBrain::CHECK_INSERTION:
        if (btSlave.isInStorage(winningIndex) || doubleTapCount > 4 ) {
          if (doubleTapCount == 0)
            brain.thoughtState = LittleBrain::SET_FOR_NEW;
          else
            brain.thoughtState = LittleBrain::MADE_IT;
        }
        else
          brain.thoughtState = LittleBrain::DOUBLE_TAP_1;
        break;

      
      // just open the grip and set for new
      case LittleBrain::MADE_IT:
        if(gripper.openTheGrip())
          brain.thoughtState = LittleBrain::SET_FOR_NEW;
        break;
        
      
      // retract the grip
      case LittleBrain::DOUBLE_TAP_1:
        if(gripper.retractTheGrip())
          brain.thoughtState = LittleBrain::DOUBLE_TAP_2;
        break;
      
      
      // close gripper to make a flat service to hammer with
      case LittleBrain::DOUBLE_TAP_2:
        if (gripper.closeTheGrip())
          brain.thoughtState = LittleBrain::DOUBLE_TAP_3;
        break;
      
      
      // extend gripper to push rod in, then check
      case LittleBrain::DOUBLE_TAP_3:
        if(gripper.extendTheGrip()) {
          brain.thoughtState = LittleBrain::CHECK_INSERTION;
        }
        break;
        

      // reset gripper position to avoid grabbing the supply tube itself instead of the rod
      case LittleBrain::SET_FOR_NEW:
       if(gripper.extendLimTheGrip()) {
         brain.thoughtState = LittleBrain::A_REVERSE_FROM_STORAGE;
         btSlave.setRadLow(false); // turn off low radiation
         follow.resetCrossCount();  // reset for line following
         doubleTapCount = 0;  // reset count
       }
       break;
      
      
       // ==================== AUTONOMY FOR REFUELING =============================
       // reverses from the storage rack until it hits the crossing
       case LittleBrain::A_REVERSE_FROM_STORAGE:
        if (follow.stopOnCrossing(driveTrain, 1, DriveTrain::BACKWARD)) {
          setDriveTrain(LittleBrain::A_REVERSE);
        }
         break;
         
       
       // goes in reverse a bit further to avoid hitting the post
       case LittleBrain::A_REVERSE:
         if (driveTrain.backupForTime()) {
           setDriveTrain(LittleBrain::A_DO_180);
         }
         break;
         
         
       // spins around.
       case LittleBrain::A_DO_180:
         // do a right turn
         if (driveTrain.turn180(false)) {
           brain.thoughtState = LittleBrain::A_CHOOSE_PATH;
           follow.resetCrossCount(); // reset for next linefollow
         }
         break;
         
        // reminder...
        /*              STORAGE SIDE
            ARRAY  0    1    2    3
                   1    2    3    4
                   1    2    3    4
            ARRAY  0    1    2    3
                      SUPPLY SIDE
        */
        
       
       // decides how many turns and crossings to proceed to supply rack
       case LittleBrain::A_CHOOSE_PATH:
         btSlave.updateArrays(); // update our arrays
         winningSupplyIndex = 0; // reset winning supply index
         // check which supply racks have rods, no optimization
         for (int i = 0; i < 4; i++) {
            if (btSlave.supplyArray[i] == 1) {
              winningSupplyIndex = i;
              break;
            }
         }
         // based on index difference, determine turns and crossings
         switch (winningSupplyIndex - winningIndex) {
           // if its straight ahead, just line follow until the peg
           case 0:
             brain.thoughtState = LittleBrain::A_LINE_FOLLOW_TO_PEG;
             break;
           // if negative...
           case -3 ... -1:
             // turn right
             shouldTurnRight = true;
             // count out crossings
             crossingCount = abs(winningSupplyIndex - winningIndex);
             brain.thoughtState = LittleBrain::A_GO_TO_CENTER;
             break;
           // if positive...
           case 1 ... 3:
             // turn left
             shouldTurnRight = false;
             // count out crossings
             crossingCount = winningSupplyIndex - winningIndex;
             brain.thoughtState = LittleBrain::A_GO_TO_CENTER;
             break;
           // shouldn't happen...
           default:
             brain.thoughtState = LittleBrain::A_LINE_FOLLOW_TO_PEG;
             break;           
         }
         follow.resetCrossCount();
         break;
         
       
       // goes to the center crossing
       case LittleBrain::A_GO_TO_CENTER:
         if (follow.stopOnCrossing(driveTrain, 1, DriveTrain::FORWARD))
           setDriveTrain(LittleBrain::A_TURN_TOWARDS_1);
         break;

       
       // turns towards the needed side
       case LittleBrain::A_TURN_TOWARDS_1:
         // turn right or left
         if (driveTrain.turn45(shouldTurnRight))
           brain.thoughtState = LittleBrain::A_DRIVE_TO_NEXT_CROSSING;
         follow.resetCrossCount();
       break;
       
       
       // drives to the predetermined crossing
       case LittleBrain::A_DRIVE_TO_NEXT_CROSSING:
         if(follow.stopOnCrossing(driveTrain, crossingCount, DriveTrain::FORWARD))
           setDriveTrain(LittleBrain::A_TURN_TO_SUPPLY);
         break;
         
       
       // turns towards the supply
       case LittleBrain::A_TURN_TO_SUPPLY:
         if (driveTrain.turn45(!shouldTurnRight)) // opposite direction from before
           brain.thoughtState = LittleBrain::A_LINE_FOLLOW_TO_PEG;
         break;
         
       
       // line follow until hitting the supply peg
       case LittleBrain::A_LINE_FOLLOW_TO_PEG:
         follow.setForwardOnCross(true); // go forward on crosses - fixes if stop command is sent while robot is on line
         if (!isBumped)
           follow.doLineFollow(driveTrain, DriveTrain::FORWARD); // switch hasn't been hit, keep going
         else { // switch has been hit
           driveTrain.halt();
           brain.thoughtState = LittleBrain::GET_NEW_ROD;
         }
         break;
       // ======================== END AUTONOMY ==================================



        // SEQUENCE FOR GETTING THINGS TO THE REACTOR
        // GET_NEW_ROD, REVERSE_FROM_SUPPLY, PREP_180, DO_180, GET_TO_CENTER
      // closes gripper around new rod
      case LittleBrain::GET_NEW_ROD:
        if (gripper.closeTheGrip()) {
          btSlave.setRadLow(false); // set radiation alerts
          btSlave.setRadHigh(true);
          brain.thoughtState = LittleBrain::GET_NEW_ROD_1;
        }
        break;
        
        
      // retracts the gripper to extract the rod
      case LittleBrain::GET_NEW_ROD_1:
        if (gripper.retractTheGrip()) {
          follow.resetCrossCount();
          brain.thoughtState = LittleBrain::REVERSE_FROM_SUPPLY;
        }
        break;
        

      // backs away from the supply rack until hitting the little line in front of it
      case LittleBrain::REVERSE_FROM_SUPPLY:
        if (follow.stopOnCrossing(driveTrain, 1, DriveTrain::BACKWARD)) {
          setDriveTrain(LittleBrain::REVERSE_AGAIN);
        }
        break;
        
        
      // reverses some more to avoid hitting peg
      case LittleBrain::REVERSE_AGAIN:
        if (driveTrain.backupForTime()) {
          setDriveTrain(LittleBrain::DO_180);
        }
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
          setDriveTrain(LittleBrain::TURN_TO_REACTOR);
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
        
        
      // lowers the arm
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
        
      // open the grip up to let go of reactor
      case LittleBrain::REFUEL_REACTOR_1:
        if (gripper.openTheGrip())
          brain.thoughtState = LittleBrain::FINISHED_REFUEL_REACTOR;
        break;
        
      
      // sets new reactor num, turns off radiation alert.
      case LittleBrain::FINISHED_REFUEL_REACTOR:
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
