
#ifndef TUMOR_AUTOMATON_H
#define TUMOR_AUTOMATON_H


#include <State.h>
#include <Parameters.h>
#include <random>

class Automaton {

private:

    State state;

    Parameters params;
    ul step = 0;
    std::random_device rd;
    std::mt19937 randomGenerator;

public:
    using coords_t = std::pair<ul, ul>;

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
     * Checks if division is possible for a cell
     * @param i
     * @param j
     * @return
     */
    bool isReadyForDivision(ul i, ul j);

    /**
     * Choose random number from 0 to prob.size() due to given probabilities.
     * If probs sums to number smaller than 1, empty choice is added.
     * For empty choice, probs.size() is returned.
     * @param probs - probabilities
     * @return
     */
    ul chooseWithProbabilites(const std::vector<double> &probs) {
        std::vector<double> sum;
        sum.push_back(probs[0].second);
        for (ul i = 1; i < probs.size(); ++i) {
            sum.push_back(sum.back() + probs[i].second);
        }
        if (sum.back() < 1.) {
            sum.push_back(1.);
        }
        std::uniform_real_distribution<double> distribution(0., 1.);
        double rand = distribution(randomGenerator);
        for (ul i = 0; i < sum.size(); ++i) {
            if (rand < sum[i]) return i;
        }
        return probs.size();
    }

    coords_t randomNeighbour(ul i, ul j);

    void birthCells(const coords_t &parent, const coords_t &child);

    /**
     * Map relative position to probability of division.
     * @param x
     * @param y
     * @return
     */
    double mapToProb(long x, long y);

public:
    const State &getState() const;

    Automaton(State &_state, Parameters &_params);

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
