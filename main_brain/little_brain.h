#ifndef little_brain_h
#define little_brain_h

class LittleBrain {
  public:
  enum littleBrainState {WAIT_FOR_BUTTON, TELEOP, GRAB, EXTRACT, BACKUP, TURN_AROUND, CHOOSE_STORAGE_RACK, LINE_FOLLOW_CROSSING, INIT_TURN, TURN, LINE_FOLLOW_TO_PEG, INSERT_STORAGE, SUPPLY};
    LittleBrain(littleBrainState state);
    void think(int state);
    
    littleBrainState thoughtState = WAIT_FOR_BUTTON;
  private:
    
};

#endif
