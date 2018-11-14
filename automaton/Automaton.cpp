
#include <Automaton.h>

#include <algorithm>

Automaton::Automaton(State& _state, Parameters& _params) : state(_state), params(_params), randomGenerator(rd()) {

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
    std::vector<std::pair<ul, ul>> readyCells;
    for (ul x = 0; x < state.gridSize; ++x) {
        for (ul y = 0; y < state.gridSize; ++y) {
            if (isReadyForDivision(x, y)) {
                readyCells.emplace_back(x, y);
            }
        }
    }
    std::shuffle(readyCells.begin(), readyCells.end(), randomGenerator);
    for (auto const &cell : readyCells) {
        auto child = randomNeighbour(cell.first, cell.second);
        if (child != cell) {

        }
    }
}

void Automaton::birthCells(const Automaton::coords_t &parent, const Automaton::coords_t &child) {

}

Automaton::coords_t Automaton::randomNeighbour(ul i, ul j) {
    ul i_start = i == 0 ? i : i - 1;
    ul i_end = (i == state.gridSize - 1) ? i : i + 1;
    ul j_start = j == 0 ? j : j - 1;
    ul j_end = (j == state.gridSize - 1) ? j : j + 1;
    std::vector<double> probs;
    std::vector<coords_t> sites;
    for (ul x = i_start; i < i_end; ++i) {
        for (ul y = j_start; j < j_end; ++j) {
            if (x != i || y != j) {
                double prob = mapToProb((long)x - i, (long)y - j);
                probs.push_back(prob);
                sites.emplace_back(x, y);
            }
        }
    }
    auto choice = chooseWithProbabilites(probs);
    if (choice < sites.size()) {
        return sites[choice];
    } else {
        return {i, j};
    }
}

void Automaton::updateStats() {
    // TODO
}

bool Automaton::isReadyForDivision(ul i, ul j) {
    return state.getW(i, j) && state.radius(i, j) < params.rMax * state.gridSize / 2 && state.gMET(i, j) == 4;
}

double Automaton::mapToProb(long x, long y) {
    constexpr double diagonalProb = 1. / (4 + 4 * 1.41421356);
    constexpr double sideProb = 1.41421356 * diagonalProb;
    if (x == 0 || y == 0) return sideProb;
    else return diagonalProb;
}

