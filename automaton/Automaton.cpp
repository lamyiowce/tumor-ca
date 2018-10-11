
#include <Automaton.h>

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
                state.CHO(i, j) = params.sCHOex;
                state.OX(i, j) = params.sOXex;
            }
}

void Automaton::diffusion() {
    // TODO
}

double Automaton::getIrradiationDose(ul step) const {
    for (auto &irradiation : params.irradiationSteps) {
        if (irradiation.first == step) return irradiation.second;
    }
    return 0.0;
}

void Automaton::irradiateTumor() {
    double dose = getIrradiationDose(step);
    if (dose > 0) {
        for (ul i = 0; i < state.gridSize; ++i) {
            for (ul j = 0; j < state.gridSize; ++j) {
                if (state.getW(i, j)) {
                    auto r0 = state.irradiation(i, j);
                    auto timeInRepair = state.timeInRepair(i, j);
                    auto effectiveIrradiation = r0 / (1 + timeInRepair / params.tau);
                    state.irradiation(i, j) = effectiveIrradiation + dose;
                    state.timeInRepair(i, j) = 0;
                }
            }
        }
    }
}

void Automaton::setLocalStates() {
    // TODO
}

// Progress clock if repair time is 0.
void Automaton::progressCellClock(ul i, ul j) {
    if (state.timeInRepair(i, j) == 0) state.proliferationTime(i, j) += params.stepTime / 3600;
}

void Automaton::metaboliseAerobicProliferation(ul i, ul j) {
    state.CHO(i, j) -= params.metabolism.aerobicProliferation.CHO;
    state.OX(i, j) -= params.metabolism.aerobicProliferation.OX;
    state.GI(i, j) += params.metabolism.aerobicProliferation.GI;
}

void Automaton::metaboliseAnaerobicProliferation(ul i, ul j) {
    state.CHO(i, j) -= params.metabolism.anaerobicProliferation.CHO;
    state.OX(i, j) -= params.metabolism.anaerobicProliferation.OX;
    state.GI(i, j) += params.metabolism.anaerobicProliferation.GI;
}

void Automaton::metaboliseAerobicQuiescence(ul i, ul j) {
    state.CHO(i, j) -= params.metabolism.aerobicQuiescence.CHO;
    state.OX(i, j) -= params.metabolism.aerobicQuiescence.OX;
    state.GI(i, j) += params.metabolism.aerobicQuiescence.GI;
}

void Automaton::metaboliseAnaerobicQuiescence(ul i, ul j) {
    state.CHO(i, j) -= params.metabolism.anaerobicQuiescence.CHO;
    state.OX(i, j) -= params.metabolism.anaerobicQuiescence.OX;
    state.GI(i, j) += params.metabolism.anaerobicQuiescence.GI;
}



void Automaton::metaboliseNutrients() {
    for (ul i = 0; i < state.gridSize; ++i) {
        for (ul j = 0; j < state.gridSize; ++j) {
            switch (state.cellState(i, j)) {
                case State::CellState::AEROBIC_PROLIFERATION:
                    progressCellClock(i, j);
                    metaboliseAerobicProliferation(i, j);
                    break;
                case State::CellState::ANAREOBIC_PROLIFERATION:
                    progressCellClock(i, j);
                    metaboliseAnaerobicProliferation(i, j);
                    break;
                case State::CellState::AEROBIC_QUIESCENCE:
                    metaboliseAerobicQuiescence(i, j);
                    break;
                case State::CellState::ANAREOBIC_QUIESCENCE:
                    metaboliseAnaerobicQuiescence(i, j);
                    break;
                default:
                    break;
            }
        }
    }
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



