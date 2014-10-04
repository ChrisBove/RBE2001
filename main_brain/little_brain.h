#ifndef little_brain_h
#define little_brain_h

class LittleBrain {
  public:
    LittleBrain();
    void think(int state);
    enum littleBrainState {TELEOP, GRAB, BACKUP, TURN_AROUND, LINE_FOLLOW_CROSSING, TURN, LINE_FOLLOW_TO_PEG, INSERT_STORAGE, SUPPLY};
  private:
    
};

#endif
