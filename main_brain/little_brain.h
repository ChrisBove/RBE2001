#ifndef little_brain_h
#define little_brain_h

class LittleBrain {
  public:
  
  enum littleBrainState {WAIT_FOR_BUTTON, TELEOP, 
    GRAB, EXTRACT, EXTRACT_1, BACKUP, BACKUP_1, TURN_AROUND, CHOOSE_STORAGE_RACK, LINE_FOLLOW_CROSSING, GO_A_BIT_FURTHER, TURN, LINE_FOLLOW_TO_PEG, INSERT_STORAGE, CHECK_INSERTION,
       DOUBLE_TAP_0, DOUBLE_TAP_1, DOUBLE_TAP_2, DOUBLE_TAP_3, DOUBLE_TAP_4, SET_FOR_NEW,
    // autnonomous storage to supply   
    A_REVERSE_FROM_STORAGE, A_REVERSE, A_DO_180, A_CHOOSE_PATH, A_GO_TO_CENTER, A_TURN_TOWARDS_1, A_DRIVE_TO_NEXT_CROSSING, A_TURN_TO_SUPPLY, A_LINE_FOLLOW_TO_PEG,
    GET_NEW_ROD, GET_NEW_ROD_1, REVERSE_FROM_SUPPLY, REVERSE_AGAIN, DO_180, GET_TO_CENTER, GET_TO_CENTER_1, TURN_TO_REACTOR, GET_TO_REACTOR, LOWER_ARM, REFUEL_REACTOR_0, REFUEL_REACTOR_1, REFUEL_REACTOR};
    
    LittleBrain(littleBrainState state);
    
    void think(int state);
    
    littleBrainState thoughtState;
  private:
    
};

#endif
