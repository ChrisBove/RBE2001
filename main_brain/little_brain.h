#ifndef little_brain_h
#define little_brain_h

class LittleBrain {
  public:
  enum littleBrainState {WAIT_FOR_BUTTON, TELEOP, 
    GRAB, EXTRACT, BACKUP, INIT_180, TURN_AROUND, CHOOSE_STORAGE_RACK, LINE_FOLLOW_CROSSING, INIT_TURN, TURN, LINE_FOLLOW_TO_PEG, INSERT_STORAGE, CHECK_INSERTION, RE_INSERT, 
    GET_NEW_ROD, REVERSE_FROM_SUPPLY, PREP_180, DO_180, GET_TO_CENTER, INIT_TURN_TO_REACTOR, TURN_TO_REACTOR, GET_TO_REACTOR, REFUEL_REACTOR};
    LittleBrain(littleBrainState state);
    void think(int state);
    
    littleBrainState thoughtState = WAIT_FOR_BUTTON;
  private:
    
};

#endif
