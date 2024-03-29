/**
 ********************************************************************************************************
 * @file    vfh.h
 * @brief   vfh class
 * @details Modified from source below into a class
 ********************************************************************************************************
 */


/*
** Virtual Field Histogram
**
** vfh.h
**
** Author: Carlos Agarie Junior
**
** This is an implementation of the Virtual Field Histogram algorithm,
** developed by J. Borenstein and Y.Koren in 1990.
*/

#ifndef vfh_h
#define vfh_h

class VFH {
  public:
    /*********************
    **
    ** Struct definitions.
    **
    *********************/
    
    /* Certainty grid. */
    struct grid {
    	int dimension;
    	int resolution;
    	int *cells;
    };
    
    /* A rangefinder measurement. */
    struct range_measure {
    	int dir; /* [degrees] */
    	unsigned long distance; /* [cm] */
    };
    
    /* Polar histogram. */
    struct hist {
    	int alpha;
    	int sectors;
    	double threshold;
    	double damping_constant;
    	double density_a;
    	double density_b;
    	int * densities;
    };
    
    /* Control signal created by the algorithm. */
    struct control_signal {
    	int dir; /* [degrees] */
    };
    
    /***********
    **
    ** Typedefs.
    **
    ***********/
    
    typedef struct grid grid_t;
    typedef struct range_measure range_measure_t;
    typedef struct control_signal control_signal_t;
    typedef struct hist hist_t;    
    
    
    /**
   * @brief   Class constructor for vfh
   * @param   grid and hist pointers
   * @return  vfh object
   */
   VFH(); //grid_t * grid, hist_t * hist);
    
    /* Control signals. */
    
    /*
    ** calculate_direction: Return the sector in hist closest to the objective
    ** direction, given that its obstacle density is less than the threshold
    ** specified in hist. The objective_direction is given in DEGREES.
    */
    int calculate_direction(hist_t * hist, int objective_direction);
    
    /* Certainty grid. */
    
    /* grid_init: Return a pointer to an empty grid. NULL otherwise. */
    grid_t * grid_init(int dimension, int resolution);
    
    /* grid_update: Update grid's cells with an array of sensor readings. */
    int grid_update(grid_t * grid, int pos_x, int pos_y, range_measure_t data);
    
    /* hist_update: Update hist with grid's information. */
    void hist_update(hist_t * hist, grid_t * grid);
    
    /* Polar Histogram. */
    
    /* hist_init: Return a pointer to a new hist. NULL otherwise. */
    hist_t * hist_init(int alpha, double threshold, double density_a,
    	double density_b);
    
    /* get_moving_window: Get a sub-grid of the grid centered in (x, y). */
    grid_t * get_moving_window(grid_t * grid, int pos_x, int pos_y, int dim);
    
  private:
    
    /**********************
    ** Function prototypes.
    **********************/
    
    /* Helpers. */
    
    /* modulo: Return x modulo m. */
    int modulo(int x, int m);
    
    /* modular_dist: Return the distance between a and b in modular arithmetic. */
    int modular_dist(int a, int b, int m);
    
};

#endif
