
#ifndef TUMOR_AUTOMATON_H
#define TUMOR_AUTOMATON_H


#include "State.h"
#include "Parameters.h"

class Automaton {
private:

    State state;

    Parameters params;
    int step = 0;
private:

    // Perform one step of the simulation.
    void advance();

    // % REPLENISHSUBSTRATE2 simply sets external CHO and Oxygen concentrations
    // % to their substrate levels.
    // Sets oxygen and CHO level of dead cells to ones specified in params.
    void replenishSubstrate();

    // % DIFFUSION2
    // %   Operates on array N to diffuse elements of
    // %   N in a Moore Neighbourhood, following Fick's 1st Law
    // %   of Diffusion (flux prop. to conc. gradient)
    void diffusion();

    // % IRRADIATETUMOR_EVO apply the correct fraction (dose) at the right time
    //%   as per the [T,R] protocol given from the present compressed protocol
    //%   string.
    //%
    //% Multi-irradiation Method/Assumptions
    //%  * we do not add R'' (new) to R' (prev. dose) damage level to the site,
    //%    instead, we assume that some degree of repair has already occured,
    //%    and so, there is an 'effective' R, R* which represents the level of
    //%    damage present in the site at the time of the new irradiation;
    void irradiateTumor();


public:
    const State &getState() const;

    Automaton(State &_state, Parameters &_params);

    void runNSteps(int nSteps);

    void setLocalStates();

    void metaboliseNutrients();

    void setGlobalStates();

    void repairCells();

    void cellDivision();

    void updateStats();
};


#endif //TUMOR_AUTOMATON_H
