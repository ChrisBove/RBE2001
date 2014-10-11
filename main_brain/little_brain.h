#ifndef little_brain_h
#define little_brain_h

class LittleBrain {
  public:
  enum littleBrainState {WAIT_FOR_BUTTON, TELEOP, 
    GRAB, EXTRACT, EXTRACT_1, BACKUP, TURN_AROUND, CHOOSE_STORAGE_RACK, LINE_FOLLOW_CROSSING, TURN, LINE_FOLLOW_TO_PEG, INSERT_STORAGE, CHECK_INSERTION, RE_INSERT,
       DOUBLE_TAP_0, DOUBLE_TAP_1, DOUBLE_TAP_2, DOUBLE_TAP_3, DOUBLE_TAP_4, 
    GET_NEW_ROD, GET_NEW_ROD_1, REVERSE_FROM_SUPPLY, REVERSE_AGAIN, DO_180, GET_TO_CENTER, TURN_TO_REACTOR, GET_TO_REACTOR, LOWER_ARM, REFUEL_REACTOR_0, REFUEL_REACTOR_1, REFUEL_REACTOR};
    LittleBrain(littleBrainState state);
    void think(int state);
    
    littleBrainState thoughtState;
  private:
    
};

#endif
