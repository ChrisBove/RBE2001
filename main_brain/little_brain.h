/**
 ********************************************************************************************************
 * @file    little_brain.h
 * @brief   little brain state class
 * @details simply holds the state of the robot.
 * @note    This was originally intended to do all the case switching that main_brain now does.
 ********************************************************************************************************
 */

#ifndef little_brain_h
#define little_brain_h

class LittleBrain {
  public:
  // defines all the possible robot states.
  enum littleBrainState {
    // teleop state, defaults to this
    TELEOP, 
    // rod storage functions
    GRAB, EXTRACT, EXTRACT_1, BACKUP, BACKUP_1, TURN_AROUND, CHOOSE_STORAGE_RACK, LINE_FOLLOW_CROSSING, GO_A_BIT_FURTHER, TURN, LINE_FOLLOW_TO_PEG, INSERT_STORAGE, RELEASE_ROD, 
    // checking if rod has been inserted into storage 
    CHECK_INSERTION, MADE_IT, DOUBLE_TAP_1, DOUBLE_TAP_2, DOUBLE_TAP_3, DOUBLE_TAP_4, SET_FOR_NEW,
    // autnonomous movement from storage racks to supply
    A_REVERSE_FROM_STORAGE, A_REVERSE, A_DO_180, A_CHOOSE_PATH, A_GO_TO_CENTER, A_TURN_TOWARDS_1, A_DRIVE_TO_NEXT_CROSSING, A_TURN_TO_SUPPLY, A_LINE_FOLLOW_TO_PEG,
    // retrieving new rod from the supply rack
    GET_NEW_ROD, GET_NEW_ROD_1, REVERSE_FROM_SUPPLY, REVERSE_AGAIN, DO_180, GET_TO_CENTER, GET_TO_CENTER_1, TURN_TO_REACTOR, GET_TO_REACTOR, LOWER_ARM, REFUEL_REACTOR_0, REFUEL_REACTOR_1, FINISHED_REFUEL_REACTOR};
    
    /**
   * @brief   Class constructor for littlebrain
   * @param   the state to start in
   * @return  LittleBrain object
   */
    LittleBrain(littleBrainState state);
    
   /**
   * @brief   would have handled changing states. Actually does nothing right now.
   * @param   state to think on
   * @return  None
   */
    void think(int state);
    
    littleBrainState thoughtState; // holds the state of the robot
    
  private:
    
};

#endif
