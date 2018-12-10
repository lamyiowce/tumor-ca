#include <Automaton.h>
#include <algorithm>

Automaton::Automaton(const State &_state, const Cycles &_cycles, const Parameters &_params, RandomEngine *randomEngine)
        : state(_state), cycles(_cycles), params(_params), randomEngine(randomEngine) {

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

void Automaton::irradiateTumor() {
    double dose = params.irradiationSteps.getIrradiationDose(step);
    if (dose <= 0) return;
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

void Automaton::setLocalStates() {
    for (ul i = 0; i < state.gridSize; ++i) {
        for (ul j = 0; j < state.gridSize; ++j) {
            if (state.getW(i, j)) {
                // Check if site should die:
                // -- too acidic, or
                // -- not enough nutrients for minimal metabolism,
                // -- completed cell cycle but not divided yet.
                if (state.GI(i, j) >= params.GIdeath
                    || state.CHO(i, j) < params.metabolism.aerobicQuiescence.CHO
                    || (state.proliferationTime(i, j) > cycles.G1time(i, j)
                                                        + cycles.Stime(i, j) + cycles.G2time(i, j)
                                                        + cycles.Mtime(i, j) + cycles.Dtime(i, j))) {

                    KillSite(i, j);
                    continue;
                }

                // Check acid conditions
                if ((state.GI(i, j) < params.GIcritical)
                    // ... and whether the site has enough neighboring vacant sites,
                    // if it is near an S-phase.
                    && !(
                        state.cellCycle(i, j) == State::CellCycle::G1
                        && (state.proliferationTime(i, j)
                            >= cycles.G1time(i, j) - 2 * params.stepTime / 3600)
                        && state.proliferationTime(i, j) < cycles.G1time(i, j)
                        && !vacantNeighbors(i, j).empty()
                )) {
                    // [matlab] try prolif --> 2B
                    if (state.OX(i, j) >= params.metabolism.aerobicProliferation.OX) {
                        if (state.CHO(i, j) >= params.metabolism.aerobicProliferation.CHO) {
                            state.cellState(i, j) = State::CellState::AEROBIC_PROLIFERATION;
                            continue;
                        } else {  // [matlab] ix_q_a
                            // TODO moze zamiast sprawdzania tylu rzeczy wgl od razu to ustawic
                            // kiedy jest cycle zmieniany, w jakiejs osobnej fladze w state?
                            // (jeszcze pozniej pojawia sie ten warunek)
                            if (state.cellCycle(i, j) == State::CellCycle::S
                                || state.cellCycle(i, j) == State::CellCycle::M
                                || state.cellCycle(i, j) == State::CellCycle::D) {

                                KillSite(i, j);
                            } else {
                                state.cellState(i, j) = State::CellState::AEROBIC_QUIESCENCE;
                            }
                            continue;
                        }
                    } else if (state.CHO(i, j)
                               >= params.metabolism.anaerobicProliferation.CHO) {  // [matlab] ix_p_an
                        state.cellState(i, j) = State::CellState::ANAREOBIC_PROLIFERATION;
                        continue;
                    }
                }
                // [matlab] ix_try_q = ix_try_qa + ix_try_qb
                if (state.OX(i, j) >= params.metabolism.aerobicQuiescence.OX) {
                    if (state.cellCycle(i, j) == State::CellCycle::S
                        || state.cellCycle(i, j) == State::CellCycle::M
                        || state.cellCycle(i, j) == State::CellCycle::D) {

                        KillSite(i, j);
                    } else {
                        state.cellState(i, j) = State::CellState::AEROBIC_QUIESCENCE;
                    }
                    continue;
                } else {  // [matlab] ix_q_ant
                    if (state.CHO(i, j) >= params.metabolism.anaerobicQuiescence.CHO) { // [matlab] ix_q_an
                        if (state.cellCycle(i, j) == State::CellCycle::S
                            || state.cellCycle(i, j) == State::CellCycle::M
                            || state.cellCycle(i, j) == State::CellCycle::D) {

                            KillSite(i, j);
                        } else {
                            state.cellState(i, j) = State::CellState::ANAREOBIC_QUIESCENCE;
                        }
                        continue;
                    } else {  // ix_dead_b
                        KillSite(i, j);
                        continue;
                    }
                }

            }
        }
    }
}

// Progress clock if repair time is 0.
void Automaton::progressCellClock(ul i, ul j) {
    if (state.timeInRepair(i, j) == 0)
        state.proliferationTime(i, j) += params.stepTime / 3600;
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
    for (ul i = 0; i < state.gridSize; ++i) {
        for (ul j = 0; j < state.gridSize; ++j) {
            if (state.getW(i, j)) {
                state.setCycleChanged(i, j, false);
                if (state.proliferationTime(i, j) <= cycles.G1time(i, j)
                    && state.cellCycle(i, j) != State::CellCycle::G1) {
                    state.cellCycle(i, j) = State::CellCycle::G1;
                    state.setCycleChanged(i, j, true);
                } else if (state.proliferationTime(i, j)
                           <= cycles.G1time(i, j) + cycles.Stime(i, j)
                           && state.cellCycle(i, j) != State::CellCycle::S) {
                    state.cellCycle(i, j) = State::CellCycle::S;
                    state.setCycleChanged(i, j, true);
                } else if (state.proliferationTime(i, j)
                           <= cycles.G1time(i, j) + cycles.Stime(i, j) + cycles.G2time(i, j)
                           && state.cellCycle(i, j) != State::CellCycle::G2) {
                    state.cellCycle(i, j) = State::CellCycle::G2;
                    state.setCycleChanged(i, j, true);
                } else if (state.proliferationTime(i, j) <=
                           cycles.G1time(i, j) + cycles.Stime(i, j) + cycles.G2time(i, j) + cycles.Mtime(i, j)
                           && state.cellCycle(i, j) != State::CellCycle::M) {
                    state.cellCycle(i, j) = State::CellCycle::M;
                    state.setCycleChanged(i, j, true);
                } else {
                    state.cellCycle(i, j) = State::CellCycle::D;
                }
            }
        }
    }
}

void Automaton::repairCells() {
    for (ul i = 0; i < state.gridSize; ++i) {
        for (ul j = 0; j < state.gridSize; ++j) {
            if ((state.cycleChanged(i, j) && (state.irradiation(i, j) > 0))
                || (state.timeInRepair(i, j) > 0)) {
                // add repair time to these sites
                // this ensures that any site in
                // repair mode has RepT > 0
                state.timeInRepair(i, j) += params.stepTime / 3600; // t in hours
                // DelayTime
                // provides a delay (in h) to the cell-phase cycle for
                // EMT6/Ro cells irradiated to level R (Gy) by some protocol.
                // find any sites which have come to the end of the repair period
                if (state.timeInRepair(i, j) >=
                    3.3414 * exp(0.1492 * state.irradiation(i, j))) {
                    if (state.irradiation(i, j) > 0) {
                        // TODO: put this in a different class/object
                        std::random_device rd;  //Will be used to obtain a seed for the random number engine
                        std::mt19937 rng(rd()); //Standard mersenne_twister_engine seeded with rd()
                        std::uniform_real_distribution<> dis(0.0, 1.0);
                        double rand = dis(rng);
                        if (rand <= 1 - exp(-0.4993 * state.irradiation(i, j))) {
                            KillSite(i, j);
                        } else {
                            // Repair cells that weren't killed
                            state.irradiation(i, j) = 0;
                            state.timeInRepair(i, j) = 0;
                        }
                    }
                }

            }

        }
    }
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
    auto permutation = randomEngine->randomPermutation(readyCells.size());
    for (auto n : permutation) {
        auto cell = readyCells[n];
        auto child = randomNeighbour(cell.first, cell.second);
        if (child != cell) {
            birthCell(cell, child);
        }
    }
}

void Automaton::birthCell(const Automaton::coords_t &parent, const Automaton::coords_t &child) {
    auto ci = child.first;
    auto cj = child.second;
    cycles.G1time(ci, cj) = randomEngine->normal(params.birthParams.G1time.mean, params.birthParams.G1time.stddev);
    cycles.Stime(ci, cj) = randomEngine->normal(params.birthParams.Stime.mean, params.birthParams.Stime.stddev);
    cycles.G1time(ci, cj) = randomEngine->normal(params.birthParams.G2time.mean, params.birthParams.G2time.stddev);
    cycles.Mtime(ci, cj) = randomEngine->normal(params.birthParams.Mtime.mean, params.birthParams.Mtime.stddev);
    cycles.Dtime(ci, cj) = randomEngine->normal(params.birthParams.Dtime.mean, params.birthParams.Dtime.stddev);

    state.setW(ci, cj, true);
    state.proliferationTime(ci, cj) = 0;
    state.cellCycle(ci, cj) = State::CellCycle::G1;

    state.proliferationTime(parent.first, parent.second) = 0;
    state.cellCycle(parent.first, parent.second) = State::CellCycle::G1;

    state.irradiation(ci, cj) = state.irradiation(parent.first, parent.second);
}

Automaton::coords_t Automaton::randomNeighbour(ul i, ul j) {
    auto vn = vacantNeighbors(i, j);
    if (vn.empty())
        return {i, j};
    std::vector<float> probs(vn.size());
    std::transform(vn.begin(), vn.end(), probs.begin(), mapToProb);
    ul choice = randomEngine->roulette(probs);
    if (choice == probs.size()) {
        return {i, j};
    } else {
        auto relativeCoords = vn[choice];
        return {i + relativeCoords.first, j + relativeCoords.second};
    }
}

void Automaton::updateStats() {
    // TODO
}

void Automaton::KillSite(ul i, ul j) {
    state.cellState(i, j) = State::CellState::DEAD;

    state.setW(i, j, false);
    // state.Dage(i, j)  = 0;  // TODO unused?
    state.proliferationTime(i, j) = 0;
    state.irradiation(i, j) = 0;
    state.timeInRepair(i, j) = 0;

    // Give off waste acids.
    state.GI(i, j) += params.siGI_n;
}

bool Automaton::isReadyForDivision(ul i, ul j) {
    return state.getW(i, j) &&
           state.radius(i, j) < params.rMax * state.gridSize / 2 &&
           state.cellCycle(i, j) == State::CellCycle::D;
}

float Automaton::mapToProb(std::pair<long, long> &relativeCoords) {
    static constexpr float diagonalProb = 1.f / (4.f + 4.f * 1.41421356f);
    static constexpr float sideProb = 1.41421356f * diagonalProb;
    if (relativeCoords.first == 0 || relativeCoords.second == 0) return sideProb;
    else return diagonalProb;
}

std::vector<std::pair<long, long>> Automaton::vacantNeighbors(ul i, ul j) {
    long i_start = i == 0 ? 0 : -1;
    long i_end = (i == state.gridSize - 1) ? 0 : 1;
    long j_start = j == 0 ? 0 : -1;
    long j_end = (j == state.gridSize - 1) ? 0 : 1;

    std::vector<std::pair<long, long>> result;
    for (long ix = i_start; ix < i_end; ++ix) {
        for (long jx = j_start; jx < j_end; ++jx) {
            if (state.getW(i + ix, j + jx) == 0) {
                result.emplace_back(ix, jx);
            }
        }
    }
    return result;
}

void Automaton::setStep(ul step) {
    this->step = step;
}

Automaton Automaton::loadFromFile(const std::string &filename, RandomEngine *re) {
    std::ifstream ifs(filename);
    nlohmann::json j = nlohmann::json::parse(ifs);
    State state(j);
    Cycles cycles(j);
    Parameters parameters(j);
    return Automaton(state, cycles, parameters, re);
}
