/* -------------------------------------------------------------------

                    TEAM KING CRAB: ROBOT CODE
                     
         Designed for use on the robot, "Philip Markus the Charming"
         
   Authors: Georgios Ardamerinos, Christopher Bove, Christopher Ellen
   
   Description: 
     This code incorporates the team code with the
     template provided by the course instructors. Added code is
     indicated as appropriate.
     
   -------------------------------------------------------------------
*/

#include <PPM.h> // includes the PPM library


PPM ppm(2); // initializes a PPM object named ppm onto pin #2

// ===================== ADDED CODE =================================

const int debug = true;  // enables/disables debug prints

#include <Servo.h>  // import libraries

  Servo left;       // Declare drive motors
  Servo right;
  
  //inititialize the motor drive constants to "stopped"
  int leftdrive = 90;  
  int rightdrive = 90;
  
  // constants for controller channels
  const int leftchannel = 3;
  const int rightchannel = 2;
//  const int retrievechannel = 6;
//  const int armchannel = 1;
  const int lineFollowChannel = 5;
  
  // motor ports
  const int leftMotorPort = 7;
  const int rightMotorPort = 6;
//  const int armPort = 8;
//  const int retrieverPort = 13;
//  const int lidPort = 9;
  
  // sensor ports
  const int RSPORT = 2;
  const int LSPORT = 0;
  const int MSPORT = 1;
//  const int armPotPort = 3;
//  const int side = 23; // low is blue coal., high is red
  
  // Sensor calibration
  const int white = 100;  // line sensor threshold is <100 and line is ~40
  const int center = 290;  // pot value with arm vert.
//  const int dump = 0;    // pot pos for dumping
  
  // constants for motor speeds and servo positions
  const int leftHalt = 96;     //stopped motor constants, trimmed for robot
  const int rightHalt = 93;
  const int lForward = 180;      // speeds to go forward for left wheel
  const int rForward = 0;    // speed to go forward for right wheel
  const int lReverse = 0;    // speed to reverse for left motor
  const int rReverse = 180;      // speed to reverse for right motor
  const int lTurn = 180;       // "left turn" speed for left motor
  const int rTurn = 180;       // "left turn" speed for right motor
  const int rTurnLM = 0;       // "right turn speed for motor Left
  const int rTurnRM = 0;       // "right turn" speed for right motor
//  const int closed = 10;       // closed pos for lid
//  const int opened = 170;      // opened pos for lid
  const int liduplim = 170;    // upper physical limit on lid
  const int lidlowlim = 10;    // lower threshold limit on lid
  
  // constants for Arm motor
  const int CWUp = 170;
  const int CWDown = 120;
  const int CCWUp = 10;
  const int CCWDown = 65;
  const int armStop = 90;
  
  // constants for lineFollow
  const int LMleftTurnLF = 60;  // left turn for Line follow motor speeds
  const int RMleftTurnLF = 40;
  const int LMrightTurnLF = 160;  // right turn for Line follow motor speeds
  const int RMrightTurnLF = 140;
  
  // vars for lineFollow
  boolean LS;    // values for sensors
  boolean MS;
  boolean RS;
  
  // constants for delays
  const int lidDelay = 50;  // time for servo to recieve command and move
  const int minDelay = 1;
  const int passOver = 750;  // time for driving over colored tape
  const int turnTime = 1000;  // time to turn to center
  const int backUpTime = 100;  // time for auto backup
  const int forwardTime = 150; // time for auto reverse
  const int forwardAgainTime = 200;
  const int controlDelay = 30; // bit o' time for servos to respond
  const int getAwayTime = 1000; // time to have robot back away from SWAG in auto
  const int timeToStatic = 500;  // time for movement to cease
  
  
// ===================== END ADDED CODE ==============================


void setup() {
  // this code runs once to initialize serial comm and attach motor pins
  Serial.begin(115200);
  
  // ===================== ADDED CODE =================================
  // attach motor ports
  left.attach(leftMotorPort);
  right.attach(rightMotorPort);
//  arm.attach(armPort);
//  retriever.attach(retrieverPort);
//  lid.attach(lidPort);
//  
//  // intialize critical motors
//  lid.write(closed); // close the lid servo to prevent falling tube!
//  delay (lidDelay);        // wait for servo to start movements
  
  // ===================== END ADDED CODE ==============================
}

// ~~~~~~~~~~~~~~~~~~~~~~~ ADDED FUNCTIONS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~   START HERE   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// ===================== halt ========================
//   Stops the robot drive motors
//   Called for Effect
void halt() {
  left.write(leftHalt);   // stop drive motors
  right.write(rightHalt);
}
// ----------------------------------------------------

// =========================== forward ==================
//   Turns on motors to drive forward
//   Called for Effect
void forward() {
  left.write(lForward);   // turn on motors to go forward
  right.write(rForward);
}
// --------------------------------------------------------

// ====================== reverse ========================
//  Turns on drive motors to go in reverse
//  Called for Effect
void reverse() {
  left.write(lReverse);  // turn on motors to go in reverse
  right.write(rReverse);
}
// -------------------------------------------------------

// ========================== turnLeft ======================
//   Turns on motors to drive left
//   Called for Effect
void turnLeft() {
  left.write(lTurn);   // turn on motors to go left
  right.write(rTurn);
}
// ---------------------------------------------------------

// ========================== turnRight ====================
//   Turns on motors to drive right
//   Called for Effect
void turnRight() {
  left.write(rTurnLM);   // turn on motors to go right
  right.write(rTurnRM);
}
// --------------------------------------------------------

// ========================== turnRightLF ======================
//   Turns on motors to drive left for line following procedures
//   This is different from turnRight since the robot needs to 
//   slow down for line following.
//   Called for Effect
void turnRightLF() {
  left.write(LMleftTurnLF);   // turn on motors to go left
  right.write(RMleftTurnLF);
}
// ---------------------------------------------------------

// ========================== turnLeftLF ====================
//   Turns on motors to drive right in line following
//   Called for Effect
void turnLeftLF() {
  left.write(LMrightTurnLF);   // turn on motors to go right
  right.write(RMrightTurnLF);
}
// --------------------------------------------------------

// =========================== Tele-drive ==================
//  drives drive motors using channels from controller
void teledrive() {
  leftdrive = ppm.getChannel(leftchannel);  // set vals
  rightdrive = ppm.getChannel(rightchannel);
  
  left.write(lForward - leftdrive);   // use chann. val to drive motors
  right.write(rForward - rightdrive);  // use offset to go forward
  
}
// ============================ retriever =================
//  drives retriever/dispensor motor using channel from cont.
//void teleretriever() {
//  retrieverdrive = ppm.getChannel(retrievechannel);  // set vals
//  
//  retriever.write(retrieverdrive);  // drive motor with val
//}

// ===========================tele-arm =======================
//  drives arm motor using values from a controller channel 
//void telearmdrive() {
//  armdrive = ppm.getChannel(armchannel);  // set vals
//  
//  arm.write(armdrive);  // drive motor with val
//}
  
// =========================== tele-lid =====================
//   drives lid servo with a single button on the controller
//   keeps the lid normally open unless we press the button
void telelid(){
  int liddrive = ppm.getChannel(lineFollowChannel);  // set values
  // if the value from channel is between the max and min thresh.
  if (((liduplim < liddrive) || (liddrive < lidlowlim))) {
    lineFollow();
  }
  else {  // otherwise
//    lid.write(closed);  // close the bucket
    teledrive();
  }
}

// ==================== Line Follow ========================
//   allows the robot to follow a line with 3 detectors
//   The sensors are analog, but we make them digital with
//   a comparison with the white setpoint value.
//   This uses a if statements to cover all states of the line sensors
//   if a state is not covered (like two sensors seeing the line)
//   we just tell the robot to keep going backward - it will make it
//   to another state. The program loops internally until it does not
//   detect a line, delaying execution of further tasks.
//   Takes and returns nothing

void lineFollow() {
  // read sensors and write values
  LS = (analogRead(LSPORT) < white);
  MS = (analogRead(MSPORT) < white);
  RS = (analogRead(RSPORT) < white);
  
  if (debug) { 
    Serial.print("LS: ");
    Serial.print(LS);
    Serial.print("MS: ");
    Serial.print(MS);
    Serial.print("RS: ");
    Serial.println(RS);
  }
  
  // while we still have some sensor on the line
//  while(!(!LS && !MS && !RS)) {
    if (debug) { Serial.println("while in line follow");}
    // if only left sensor is tripped
    if((LS && !MS && !RS)) {
      turnRightLF();  // turn right with LF vals
    }
    else {  // otherwise
      // if right sensor is tripped,
      if (!LS && !MS && RS) {
        turnLeftLF();  // turn left for line follow
      }
      else reverse();  // if any other case, go in reverse
      // Note: the line sensors are on the back of the robot
    }
    // set new values
    LS = (analogRead(LSPORT) < white);
    MS = (analogRead(MSPORT) < white);
    RS = (analogRead(RSPORT) < white);
    
//  }  // end while
  // we are now off the line
//  halt();  // stop the drive motors 
}
// -------------------------------------------------------

// ====================== Move Arm =======================
//   simply moves the arm to the preset position in auto.
//   uses a simple controller - arm torques make proportional
//   control difficult. When rising, arm gets full power,
//   when lowering, arm gets less power.
//   The only time the arm auto moves is to dump the tube in auto,
//   therefore there was no necessity to enabling movements the other way,
//   though it could be implemented as it was in Lab 4. Time did not allow.

//void moveArm() {
//  // while we are not at dump position,
//  while(analogRead(armPotPort) > dump) {
//    // if arm is raising to the center from start position
//    if (analogRead(armPotPort) > center) {
//      arm.write(CWUp);  // use the speed to go up clockwise
//      if (debug) { Serial.println("if"); }
//    }
//    // otherwise, arm is on the falling side of center
//    else {
//      arm.write(CWDown);  // use downward speed to go clockwise
//    if (debug) {Serial.println("else"); }
//    }
//  }  // end while
//  arm.write(armStop);  // stop the arm motor
//}
// -----------------------------------------------------

// ====================== getOutOfBox =======================
//  script to drive robot out of the alliance box and until
//  reaching the line to follow using light sensors.
//void getOutOfBox() {
//  reverse();  // start going backwards!
//  delay(passOver); // wait a bit to drive over colored line...
//  LS = (analogRead(LSPORT) < white);  // before checking sensors...
//  MS = (analogRead(MSPORT) < white);
//  RS = (analogRead(RSPORT) < white);
//  // to see if we hit the white line yet
//  // while we don't see any white...
//  while(!(LS || MS || RS)) {
//    delay(minDelay);  // wait a bit
//    LS = (analogRead(LSPORT) < white);  // read sensors again
//    MS = (analogRead(MSPORT) < white);
//    RS = (analogRead(RSPORT) < white);
//  }  // end while
//  halt(); // we are out of the box and on the line, stop!
//}
// ------------------------------------------------------------

// ====================== turnToCenter =======================
//  script to turn the robot until it is parallel with the line
//void turnToCenter() {
//  // if we are on the blue side
//  if (digitalRead(side) == LOW) { // IF BLUE
//    // turn on motors to go right
//    turnRight();
//    delay(turnTime);  // wait to turn a bit
//  }
//  else {  // else this is the red side, so...
//    
//    // turn on motors to go left
//    turnLeft();
//    delay(turnTime);  // wait to turn a bit
//  }
//  // after the delay, start keep turning while we don't see the line
//  while(!(LS || MS || RS)) {
//    delay(minDelay);  // wait a bit
//    if (debug) { Serial.println("turning to center"); }
//    LS = (analogRead(LSPORT) < white);  // update sensor vals
//    MS = (analogRead(MSPORT) < white);
//    RS = (analogRead(RSPORT) < white);
//  }  // end while
//  // we are centered!
//  halt();  // stop the robot
//}
// ----------------------------------------------------------------

//// ====================== moveBackward =======================
////  script to move robot back during auto
//void moveBackward() {
//  forward();  // robot is going in the opposite direction from arm...
//  delay(backUpTime);  // wait 
//  halt();             // stop
//  delay(controlDelay);  // wait for motors to get command
//}
//// ====================== moveForward =======================
////  script to move robot forward during auto
//void moveForward() {
//  reverse();  // go forwards by driving motor backwards
//  delay(forwardTime);
//  halt();
//  delay(controlDelay);
//}
//// ====================== openBox =======================
////  script to open the lid during auto
//void openBox() {
//  lid.write(opened);  // drive lid servo
//}
//// ====================== moveForwardAgain =======================
////  script for second move forward in auto
//void moveForwardAgain() {
//  reverse();
//  delay(forwardAgainTime);  // wait for time
//  halt();
//}
//// ====================== getAway =======================
////  script to drive robot away from SWAG after dumping
//void getAway() {
//  forward();
//  delay(getAwayTime);  // drive until you've gone far enough
//  halt();
//}
//
//
//// =======================================================================
////                         crabAutonomous
//// called for our team's auto, loops internally until completed
//void crabAutonomous () {
//  
//      // We only want this sequence to run once, the end is arm at pot = dump, 
//      // so we check to see if the pot is at zero to know when to stop executing this 
//      // loop to move to Tele when timer has stopped
//      
//      // while arm is not at desired dump position
//      while(analogRead(armPotPort) != dump) {
//        
//        getOutOfBox();  // get out of alliance box
//        if(debug) {Serial.println("out of box");}
//        
//        turnToCenter();  // rotate to go parallel with the line
//        if(debug) {Serial.println("turned to center");}
//        
//        // run line follow several times, line follow will stop when the sensors 
//	 // detect nothing
//// sometimes the sensors read a false while driving, but we allow a few 
//// mistakes
//        // to be made before we really stop on the end.
//        for(int i = 0; i <200; i++) {
//           lineFollow();
//        }
//        if(debug) {Serial.println("line followed");}
//        halt();  // stop
//        
//        delay(timeToStatic);  // wait for movement to stop
//        moveBackward();       // move back to allow arm to not hit backboard
//        if(debug) {Serial.println("moved back");}
//        
//        delay(timeToStatic);
//        moveArm();  // move the arm to position
//        if(debug) {Serial.println("moved arm to 0");}
//        
//        delay(timeToStatic);
//        moveForward();  // move forward again to close distance between box and SWAG
//        if(debug) {Serial.println("moved forward");}
//        
//        openBox();  // open the box lid
//        if(debug) {Serial.println("opened box");}
//        
//        delay(timeToStatic);
//        moveForwardAgain();  // move forward to make sure everything comes out
//        if(debug) {Serial.println("moved forward again");}
//        
//        delay(timeToStatic);
//        getAway();  // move robot away from SWAG to not touch the TUBE
//        if(debug) {Serial.println("got away");}
//      
//      }  // end while, we are off the line and done the script
//}
// ```````````````````````````````````````````````````````````````````````````````

// ======================================================================= 
//                               crabTeleop
// called for our team's teleop, executes each iteration of main teleop, 
// all functions execute once per cycle
void crabTeleop () {
  
  // Teleop calls the following functions, each executes once per teleop loop
    teledrive();      // drive drive motors
//    teleretriever();  // control retriever motor
//    telearmdrive();   // control arm motor
    telelid();        // control lid servo
}
// ````````````````````````````````````````````````````````````````````````````````````

// ~~~~~~~~~~~~~~~~~~~~~~ END ADDED FUNCTIONS ~~~~~~~~~~~~~~~~~~~~~~~~~~~


//void autonomous(unsigned long time) // function definition
//{
//  while(0==ppm.getChannel(1)){ // waits for controller to be turned on
//   Serial.println("waiting for controller");
//    }
//    unsigned long startTime=millis(); // sets start time of autonomous
//    time=time * 1000;  // modifies milliseconds to seconds
//    while (millis()-startTime<=time)// compares start time to time entered in the autonomous function
//    {
//     
//      // Enter Autonoumous User Code Here
//      
//      // ===================== START ADDED CODE =================================
//      
//      // run our code, which loops internally, it will finish and not run again,
//      // allowing auto loop to start executing again
//
//      crabAutonomous();
//      
//      // ======================== END ADDED CODE ==================================
//      
//      Serial.println("Autonoumous"); //prints Autonomous over serial (usb com port)
//      delay(50); //delay to prevent spamming the serial port  
//    }
//    
//  }
//void teleop(unsigned long time){ // function definition
//  unsigned long startTime2=millis(); // sets start time of teleop
//  time=time * 1000; // modifies milliseconds to seconds
//  while(millis()-startTime2<=time) // compares start time to time entered in the teleop function
//    {
//     
//    // Enter Teleop User Code Here
//    
//    // ===================== START ADDED CODE =================================
//    
//    crabTeleop();
//    
//    // ======================== END ADDED CODE ===================================
//  
//    Serial.println("TeleOp"); //prints Teleop over serial (usb com port)
//    delay(50); //delay to prevent spamming the serial port 
//
//  }
//  exit(0); // exits program
//  }

void loop() {
  telelid();
 
}

