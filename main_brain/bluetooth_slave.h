// this code is modified from Craig Putnam's sketch_slave_bluetooth_sample code

#ifndef bluetooth_slave_h
#define bluetooth_slave_h

#include "Arduino.h"
#include <BluetoothClient.h>
#include <BluetoothMaster.h>
#include <ReactorProtocol.h>
#include <TimerOne.h>

#define teamNumber 21
#define onboardLED 13
#define radiationLED 30

class BluetoothSlave {
  
  public:
    BluetoothSlave();
    void setupBluetooth();
    void goTime();
    void timer1ISR();
    void doTimerInterrupt();

    void updateArrays();
    void update();
    int isInStorage(int index);
    
    void setRadLow(bool enable);
    void setRadHigh(bool enable);
    
    bool storageArray[4];
    bool supplyArray[4];
    
    bool stopMovement = false;
    bool resumeMovement = true;
    
  private:
    void sendHighRadiation();
    void sendLowRadiation();
    
    // set up module-wide variables
    // these are 'volatile' as they are referenced in as well as outside of ISRs
    volatile unsigned char tickCount;              // elapsed time in ticks (timer interrupts)
    volatile unsigned long elapsedTime;            // ...and in seconds
    volatile unsigned char hbCount;                // ticks since last heartbeat message was sent
    volatile unsigned char radCount;               // ticks since last radiation message was sent
    volatile boolean sendHB;                       // flag indicating it is time to send a heartbeat message
    volatile boolean sendRad;                      // flag indicating it is time to send a radiation message
    volatile boolean go;                           // flag indicating it is OK to start transmitting messages
    
    bool enableRadHigh = false;
    bool enableRadLow = false;
    
    // these need to be module-wide variables (so they persist across iterations of loop()
    byte storageData;                              // holds the bitmask for the storage tubes
    byte supplyData;                               // ... and the supply tubes
    
    byte pkt[10];                                // allocate memory for the bytes in the packet
    int sz;                                      // holds the size of the message (in bytes)
    byte type;                                   // hold the message type id
    byte data1[3];                               // holds any data associated with a message
};

#endif
