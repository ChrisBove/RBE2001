/**
 ********************************************************************************************************
 * @file    bluetooth_slave.h
 * @brief   Bluetooth control class
 * @details Handles bluetooth functions and data
 * @note    this code is modified from Craig Putnam's sketch_slave_bluetooth_sample code
 ********************************************************************************************************
 */

#ifndef bluetooth_slave_h
#define bluetooth_slave_h

/*** INCLUDE FILES ***/
#include "Arduino.h"
#include <BluetoothClient.h>
#include <BluetoothMaster.h>
#include <ReactorProtocol.h>
#include <TimerOne.h>

#define teamNumber      21

class BluetoothSlave {
  
  public:
     /**
   * @brief   Class constructor for bluetooth.
   * @param   pins for onboard led and radiation led
   * @return  bluetooth slave object
   */
    BluetoothSlave(int onboardLED, int radiationLED);
    /**
   * @brief   setups bluetooth, inits vars, and attaches pins
   * @param   None
   * @return  None
   */
    void setupBluetooth();
   /**
   * @brief   flags bluetooth to start sending heart beats and sending/receiving mssgs
   * @param   None
   * @return  None
   */
    void goTime();
   /**
   * @brief   should happen on 100ms timer interrupt, services heart beat, radiation messages, timer
   * @param   None
   * @return  None
   */
    void doTimerInterrupt();
   /**
   * @brief   refreshes arrays with latest bluetooth data
   * @param   None
   * @return  None
   */
    void updateArrays();
   /**
   * @brief   runs updates for data gathering if we have data
   * @param   None
   * @return  None
   */
    void update();
   /**
   * @brief   returns 1 if requested index has a tube in storage
   * @param   index of storage array to check
   * @return  1 if index has a tube in it
   */
    int isInStorage(int index);
   /**
   * @brief   flags to send low radiation messages, lights warning led appropriately
   * @param   true to turn on low rad, false to turn off
   * @return  None
   */
    void setRadLow(bool enable);
   /**
   * @brief   flags to send high radiation messages, lights warning led appropriately
   * @param   true to turn on high rad, false to turn off
   * @return  None
   */
    void setRadHigh(bool enable);
   /**
   * @brief   
   * @param   true to turn on high rad, false to turn off
   * @return  None
   */
    bool storageArray[4]; // array of storage tubes, 1 is occupied 0 is empty
    bool supplyArray[4];  // array of supply tubes
    
    bool stopMovement = false;  // set this flag in a client if movement should be stopped
    bool resumeMovement = true; // set this flag in a client if movement should resume
    
  private:
   /**
   * @brief   sends high radiation message
   * @param   None
   * @return  None
   */
    void sendHighRadiation();
   /**
   * @brief   sends low radiation message
   * @param   None
   * @return  None
   */
    void sendLowRadiation();
    
    // set up module-wide variables
    // these are 'volatile' as they are referenced in as well as outside of ISRs
    volatile unsigned char tickCount;              // elapsed time in ticks (timer interrupts)
    volatile unsigned char hbCount;                // ticks since last heartbeat message was sent
    volatile unsigned char radCount;               // ticks since last radiation message was sent
    volatile boolean sendHB;                       // flag indicating it is time to send a heartbeat message
    volatile boolean sendRad;                      // flag indicating it is time to send a radiation message
    volatile boolean go;                           // flag indicating it is OK to start transmitting messages
    
    bool enableRadHigh = false; // local storage of whether to enable radiation messages
    bool enableRadLow = false;
    
    // these need to be module-wide variables (so they persist across iterations of loop()
    byte storageData;                              // holds the bitmask for the storage tubes
    byte supplyData;                               // ... and the supply tubes
    
    byte pkt[10];                                // allocate memory for the bytes in the packet
    int sz;                                      // holds the size of the message (in bytes)
    byte type;                                   // hold the message type id
    byte data1[3];                               // holds any data associated with a message
    
    int _onboardLED;  // pins of LEDs
    int _radiationLED;
};

#endif
