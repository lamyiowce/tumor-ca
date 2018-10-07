
#include "Automaton.h"

Automaton::Automaton(State& _state, Parameters& _params) : state(_state), params(_params){

}

const State &Automaton::getState() const {
    return state;
}

void Automaton::runNSteps(int nSteps) {
    replenishSubstrate();
    for (int i = 0; i < nSteps; i++)
        advance();
}

void Automaton::advance() {
    diffusion();
    irradiateTumor();
    setLocalStates();
    metaboliseNutrients();
    setGlobalStates();
    repairCells();
    cellDivision();
    updateStats();
    // TODO: finish check
}

void Automaton::replenishSubstrate() {
    for (ul i = 0; i < state.gridSize; i++)
        for (ul j = 0; j < state.gridSize; j++)
            if (!state.getW(i, j)) {
                state.getCHO(i, j) = params.sCHOex;
                state.getOX(i, j) = params.sOXex;
            }
}

void Automaton::diffusion() {
    // TODO
}

void Automaton::irradiateTumor() {
    double dose = params.irradiationPerStep[step];
    if (dose > 0) {
        for (ul i = 0; i < state.gridSize; i++) {
            for (ul j = 0; j < state.gridSize; j++) {
                if (state.getW(i, j)) {
                    auto r0 = state.getIrradiation(i, j);
                    auto timeInRepair = state.getTimeInRepair(i, j);
                    auto effectiveIrradiation = r0 / (1 + timeInRepair / params.tau);
                    state.getIrradiation(i, j) = effectiveIrradiation + dose;
                    state.getTimeInRepair(i, j) = 0;
                }
            }
        }
    }
}

void Automaton::setLocalStates() {
    // TODO
}

void Automaton::metaboliseNutrients() {
    // TODO
}

void Automaton::setGlobalStates() {
    // TODO
}

void Automaton::repairCells() {
    // TODO
}

void Automaton::cellDivision() {
    // TODO
}

void Automaton::updateStats() {
    // TODO
}

