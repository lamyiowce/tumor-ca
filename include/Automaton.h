
#ifndef TUMOR_AUTOMATON_H
#define TUMOR_AUTOMATON_H


#include <State.h>
#include <Parameters.h>
#include <Cycles.h>

#include <math.h>

class Automaton {
private:
    State state;
	Cycles cycles;

    Parameters params;
    ul step = 0;
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

    /**
     * Checks the irradiation dose for current step
     * @param step
     * @return irradiation dose
     */
    double getIrradiationDose(ul step) const;

    /**
     * Progress proliferation clock.
     * @param i first coordinate
     * @param j second coordinate
     */
    void progressCellClock(ul i, ul j);

    /**
     * Metabolise for a cell in aerobic proliferation state.
     * @param i first coordinate
     * @param j second coordinate
     */
    void metaboliseAerobicProliferation(ul i, ul j);

    /**
     * Metabolise for a cell in anaerobic proliferation state.
     * @param i first coordinate
     * @param j second coordinate
     */
    void metaboliseAnaerobicProliferation(ul i, ul j);

    /**
     * Metabolise for a cell in aerobic quiescence state.
     * @param i first coordinate
     * @param j second coordinate
     */
    void metaboliseAerobicQuiescence(ul i, ul j);

    /**
     * Metabolise for a cell in anaerobic quiescence state.
     * @param i first coordinate
     * @param j second coordinate
     */
    void metaboliseAnaerobicQuiescence(ul i, ul j);

    /**
     * KILLSITE executes the cell-death program for site indicated
     *   by the index arguments.
     * @param i first coordinate
     * @param j second coordinate
     */
    void KillSite(ul i, ul j);

	bool hasVacantNeighbors(ul i, ul j);

public:
    const State &getState() const;

    Automaton(State &_state, Cycles &_cycles, Parameters &_params);

    void runNSteps(int nSteps);

    void setLocalStates();

    /**
     * Calculate new nutrients values in all living sites. Progress proliferation clock.
     * MetaboliseNutrients in matlab
     */
    void metaboliseNutrients();

    void setGlobalStates();

    void repairCells();

    void cellDivision();

    void updateStats();
};

#endif //TUMOR_AUTOMATON_H
