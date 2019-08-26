#ifndef TUMOR_AUTOMATON_H
#define TUMOR_AUTOMATON_H

#include <State.h>
#include <Parameters.h>
#include <Cycles.h>
#include <RandomEngine.h>

class Automaton {
public:
    State state;
	Cycles cycles;

    Parameters params;
    ul step = 1; // To be consistent with MATLAB simulation.
    RandomEngine *randomEngine;

public:
    using coords_t = std::pair<ul, ul>;


    struct LatticeCopy {
        ul borderedH;
        ul borderedW;
        std::array<grid<double_p >, 2> CHO;
        std::array<grid<double_p>, 2> OX;
        std::array<grid<double_p>, 2> GI;

        LatticeCopy(ul borderedH, ul borderedW)
        : borderedH(borderedH)
        , borderedW(borderedW)
        , CHO{grid<double_p>(borderedH * borderedW), grid<double_p>(borderedH * borderedW)}
        , OX{grid<double_p>(borderedH * borderedW), grid<double_p>(borderedH * borderedW)}
        , GI{grid<double_p>(borderedH * borderedW), grid<double_p>(borderedH * borderedW)}
        {}
    };

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
    static double mapToProb(std::pair<long, long> &relativeCoords);

    void numericalDiffusion(ul r, ul c, const grid<double_p> &choCopy, const grid<double_p> &oxCopy,
                                const grid<double_p> &giCopy, grid<double_p> &choResult, grid<double_p> &oxResult,
                                grid<double_p> &giResult, ul gridW);

    std::pair<double_p, double_p> sumNeighbors(ul r, ul c, const grid<double_p> &values, ul gridW);

public:
    const State &getState() const;

    Cycles getCycles() const;

    Automaton(const State &_state, const Cycles &_cycles, const Parameters &_params,
                  RandomEngine *randomEngine, ul _step = 1);

    Automaton(const Automaton& other) = default;
    /**
     * Sets external CHO and Oxygen concentrations to their substrate levels.
     * Sets oxygen and CHO level of dead cells to ones specified in params.
     */
    void replenishSubstrate();

    void runNSteps(int nSteps);

    /// Performs one step of the simulation.
    void advance();

    /**
     * Diffuses glucose, oxygen and TODO
     * in the lattice, following Fick's First Law
     */
    void diffusion();

    /**
     * Localize living cells on the lattice
     * @return (coordinates of the middle of the tumor, maximal distance of the living cell)
     */
    std::pair<coords_t, ul> findTumor();

    /**
     * Calculate coordinates and dimensions of the sublattice containing the tumor
     * @param mid - coordinates of the middle of the tumor
     * @param maxDist - maximal distance of the living cell
     * @return ((sublattice row, sublattice column), (sublattice height, sublattice width))
     */
    std::pair<Automaton::coords_t, Automaton::coords_t> findSubLattice(coords_t mid, ul maxDist);

    /**
     * Construct temporal grids for calculating diffusion.
     * @param borderedH
     * @param borderedW
     * @param subLatticeR
     * @param subLatticeC
     * @return
     */
    LatticeCopy copySubLattice(ul borderedH, ul borderedW, ul subLatticeR, ul subLatticeC);

    void calculateDiffusion(LatticeCopy &copy, std::vector<coords_t> borderSites, ul rounds);

    std::vector<coords_t> findBorderSites(ul borderedH, ul borderedW, ul maxDist);

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

    static Automaton loadFromFile(const std::string &filename, RandomEngine * re);

    void setIrradiationProtocol(IrradiationProtocol protocol);

    void setRandomEngine(RandomEngine* re);

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
