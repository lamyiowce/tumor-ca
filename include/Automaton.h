#ifndef TUMOR_AUTOMATON_H
#define TUMOR_AUTOMATON_H

#include <State.h>
#include <Parameters.h>
#include <Cycles.h>
#include <RandomEngine.h>

class Automaton {
private:
    State state;
	Cycles cycles;

    Parameters params;
    ul step = 1; // To be consistent with MATLAB simulation.
    RandomEngine *randomEngine;

public:
    using coords_t = std::pair<ul, ul>;

private:

    /// Performs one step of the simulation.
    void advance();

    /**
     * Progress proliferation clock.
     * @param r first coordinate
     * @param c second coordinate
     */
    void progressCellClock(ul r, ul c);

    /**
     * Metabolise for a cell in aerobic proliferation state.
     * @param r first coordinate
     * @param c second coordinate
     */
    void metaboliseAerobicProliferation(ul r, ul c);

    /**
     * Metabolise for a cell in anaerobic proliferation state.
     * @param r first coordinate
     * @param c second coordinate
     */
    void metaboliseAnaerobicProliferation(ul r, ul c);

    /**
     * Metabolise for a cell in aerobic quiescence state.
     * @param r first coordinate
     * @param c second coordinate
     */
    void metaboliseAerobicQuiescence(ul r, ul c);

    /**
     * Metabolise for a cell in anaerobic quiescence state.
     * @param r first coordinate
     * @param c second coordinate
     */
    void metaboliseAnaerobicQuiescence(ul r, ul c);

    /**
     * KILLSITE executes the cell-death program for site indicated
     *   by the index arguments.
     * @param r first coordinate
     * @param c second coordinate
     */
    void KillSite(ul r, ul c);

	std::vector<std::pair<long, long>> vacantNeighbors(ul r, ul c);

    /**
     * Checks if division is possible for a cell
     * @param r
     * @param c
     */
    bool isReadyForDivision(ul r, ul c);

    /**
     * Choose random vacant neighbour of a cell
     * @param r first coordinate
     * @param c second coordinate
     */
    coords_t randomNeighbour(ul r, ul c);

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

    void numericalDiffusion(ul r, ul c, const grid<double> &choCopy, const grid<double> &oxCopy,
                                const grid<double> &giCopy, grid<double> &choResult, grid<double> &oxResult,
                                grid<double> &giResult, ul gridW);

    std::pair<double, double> sumNeighbours(ul r, ul c, const grid<double> &values, ul gridW);

public:
    const State &getState() const;

    Automaton(const State &_state, const Cycles &_cycles, const Parameters &_params,
                  RandomEngine *randomEngine, ul _step = 1);


    /**
     * Sets external CHO and Oxygen concentrations to their substrate levels.
     * Sets oxygen and CHO level of dead cells to ones specified in params.
     */
    void replenishSubstrate();

    void runNSteps(int nSteps);

    // % DIFFUSION2
    // %   Operates on array N to diffuse elements of
    // %   N in a Moore Neighbourhood, following Fick's 1st Law
    // %   of Diffusion (flux prop. to conc. gradient)
    void diffusion();

    /**
     * Apply the correct fraction (dose) at the right time
     */
    void irradiateTumor();

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

    static Automaton loadFromFile(const std::string &filename, RandomEngine * re);

    /**
     * Save automaton state to csv file. Vector values separated by a comma ','. Data format is as follows:
     * step
     * W
     * CHO
     * OX
     * GI
     * timeInRepair
     * irradiation
     * cellState
     * cellCycle
     * proliferationTime
     * cycleChanged
     * G1time
	 * Stime
	 * G2time
	 * Mtime
	 * Dtime
     * @param filename
     */
    void saveStateToFile(const std::string &filename);
};

#endif //TUMOR_AUTOMATON_H
