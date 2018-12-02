
#ifndef TUMOR_AUTOMATON_H
#define TUMOR_AUTOMATON_H


#include <State.h>
#include <Parameters.h>
#include <random>
#include <Cycles.h>
#include <bits/unique_ptr.h>
#include "RandomEngine.h"

#include <random>
#include <math.h>


class Automaton {
private:

    State state;
	Cycles cycles;

    Parameters params;
    ul step = 0;
    RandomEngine *randomEngine;

public:
    using coords_t = std::pair<ul, ul>;

private:

    /// Performs one step of the simulation.
    void advance();

    /**
     * Sets external CHO and Oxygen concentrations to their substrate levels.
     * Sets oxygen and CHO level of dead cells to ones specified in params.
     */
    void replenishSubstrate();

    // % DIFFUSION2
    // %   Operates on array N to diffuse elements of
    // %   N in a Moore Neighbourhood, following Fick's 1st Law
    // %   of Diffusion (flux prop. to conc. gradient)
    void diffusion();

    /**
     * Apply the correct fraction (dose) at the right time
     */
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

	std::vector<std::pair<long, long>> vacantNeighbors(ul i, ul j);

    /**
     * Checks if division is possible for a cell
     * @param i
     * @param j
     */
    bool isReadyForDivision(ul i, ul j);

    /**
     * Choose random vacant neighbour of a cell
     * @param i first coordinate
     * @param j second coordinate
     */
    coords_t randomNeighbour(ul i, ul j);

    /**
     * Setup a new cell
     * @param parent site of a parent cell
     * @param child site for the new cell
     */
    void birthCell(const coords_t &parent, const coords_t &child);

    /**
     * Map relative position to probability of division.
     * @param x
     * @param y
     * @return
     */
    static float mapToProb(std::pair<long, long> &relativeCoords);

public:
    const State &getState() const;

    Automaton(State &_state, Cycles &_cycles, Parameters &_params, RandomEngine *randomEngine);

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
