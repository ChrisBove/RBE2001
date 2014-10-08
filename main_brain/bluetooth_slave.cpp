// this code is modified from Craig Putnam's sketch_slave_bluetooth_sample code

#include <BluetoothClient.h>
#include <BluetoothMaster.h>
#include <ReactorProtocol.h>
#include <TimerOne.h>

#include "bluetooth_slave.h"

ReactorProtocol pcol(byte(teamNumber));         // instantiate the protocol object and set the robot/team source address
BluetoothClient bt;                            // instantiate a Bluetooth client object
BluetoothMaster btmaster;                      // ...and a master object

BluetoothSlave::BluetoothSlave() {
  
}

void BluetoothSlave::setupBluetooth() {
  Serial1.begin(115200);
  
  // init various variables and flags
  go = false;
  tickCount = 0;
  hbCount = 0;
  elapsedTime = 0;
  sendHB = false;
  sendRad = false;
  
  pinMode(onboardLED, OUTPUT);
  digitalWrite(onboardLED, LOW);
  
}

void BluetoothSlave::goTime() {
  go = true;
  
}

void BluetoothSlave::doTimerInterrupt() {
  // we are here because the timer expired and generated an interrupt
  tickCount++;                                 // increment the 100ms tick counter
  hbCount++;                                   // increment the heartbeat counter
  radCount++;                                  // increment radiation counter
  if (tickCount >= 10) {                       // do the following once a second
    tickCount = 0;                             // reset the tick counter
    elapsedTime++;			       // increment the elapsed time counter (in seconds)
  }
  if (hbCount >= 20) {                         // do the following every other second
    hbCount = 0;                               // reset the heartbeat counter
    sendHB = true;                             // note it is time to send a heartbeat message
  }
  if (radCount >= 25) {
    radCount = 0;
    sendRad = true;
  }
}

void BluetoothSlave::update() {

  if (sendHB && go) {                          // execute if GO flag is set and it's time to generate a heartbeat message
    sendHB = false;                            // clear the heartbeat flag

    // generate and send the heartbeat message    
    digitalWrite(onboardLED, !digitalRead(onboardLED));  // flip the state of the HB LED
    pcol.setDst(0x00);			       // this will be a broadcast message
    sz = pcol.createPkt(0x07, data1, pkt);     // create a packet using the heartbeat type ID (there is no data)
    btmaster.sendPkt(pkt, sz);                 // send to the field computer

  }
  else if (sendRad && go) {
    sendRad = false;
    if (enableRadHigh)
      sendHighRadiation();
    if (enableRadLow)
      sendLowRadiation();
  }
  
  // time to read messages
  // each time through loop() we check to see if a message is present

  // attempt to read a message (packet)
  // the only messages returned are those that are broadcast or sent specifically to this robot
  if (btmaster.readPacket(pkt)) {              // if we have received a message
    if (pcol.getData(pkt, data1, type)) {      // see if we can extract the type and data
      switch (type) {                          // process the message based on the type
      case 0x01:                               // received a storage tube message
        storageData = data1[0];                // extract and save the storage-related data (the byte bitmask)
        break;
      case 0x02:                               // received a supply tube message
        supplyData = data1[0];                 // extract and save the supply-related data (the byte bitmask)
        break;
      // TODO - add other cases for other data
      case 0x04:
        // stop movement case!
        resumeMovement = false;
        stopMovement = true;
        break;
      case 0x05:
        // resume movement case!
        resumeMovement = true;
        stopMovement = false;
        break;
        
      default:                                 // ignore other types of messages
        break;
      }
    }
  }
}

void BluetoothSlave::setRadLow(bool enable) {
  if (enable)
    enableRadLow = true;
  else
    enableRadLow = false;
}

void BluetoothSlave::setRadHigh(bool enable) {
  if (enable)
    enableRadHigh = true;
  else
    enableRadHigh = false;
}

void BluetoothSlave::sendHighRadiation() {
    // example of sending a radiation alert message (new fuel rod)
    pcol.setDst(0x00);			       // this will be a broadcast message
    data1[0] = 0xFF;                           // indicate a new fuel rod
    sz = pcol.createPkt(0x03, data1, pkt);     // create a packet using the radiation alert type ID (1 byte of data used this time)
    btmaster.sendPkt(pkt, sz);                 // send to the field computer
}

void BluetoothSlave::sendLowRadiation() {
    // send a message for a low radiation alert
    pcol.setDst(0x00);			       // this will be a broadcast message
    data1[0] = 0x2C;                           // indicate a depleted fuel rod
    sz = pcol.createPkt(0x03, data1, pkt);     // create a packet using the radiation alert type ID (1 byte of data used this time)
    btmaster.sendPkt(pkt, sz);                 // send to the field computer
}

/*              STORAGE SIDE
    ARRAY  0    1    2    3
           1    2    3    4
           1    2    3    4
    ARRAY  0    1    2    3
              SUPPLY SIDE
*/
void BluetoothSlave::updateArrays() {
    storageArray[0] = storageData & 0x01;
    storageArray[1] = storageData & 0x02;
    storageArray[2] = storageData & 0x04;
    storageArray[3] = storageData & 0x08;
    
    supplyArray[0] = supplyData & 0x01;
    supplyArray[1] = supplyData & 0x02;
    supplyArray[2] = supplyData & 0x04;
    supplyArray[3] = supplyData & 0x08;

}
