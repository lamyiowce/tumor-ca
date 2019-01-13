#include <Automaton.h>
#include <algorithm>

Automaton::Automaton(const State &_state, const Cycles &_cycles, const Parameters &_params,
                     RandomEngine *randomEngine, ul _step)
        : state(_state), cycles(_cycles), params(_params), step(_step), randomEngine(randomEngine) {

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
    for (ul r = 0; r < state.gridSize; r++)
        for (ul c = 0; c < state.gridSize; c++)
            if (!state.getW(r, c)) {
                state.CHO(r, c) = params.sCHOex;
                state.OX(r, c) = params.sOXex;
            }
}

void Automaton::diffusion() {
    // TODO
}

void Automaton::irradiateTumor() {
    double dose = params.irradiationSteps.getIrradiationDose(step);
    if (dose <= 0) return;
    for (ul r = 0; r < state.gridSize; ++r) {
        for (ul c = 0; c < state.gridSize; ++c) {
            if (state.getW(r, c)) {
                const auto r0 = state.irradiation(r, c);
                const auto timeInRepair = state.timeInRepair(r, c);
                const auto effectiveIrradiation = r0 / (1 + timeInRepair / params.tau);
                state.irradiation(r, c) = effectiveIrradiation + dose;
                state.timeInRepair(r, c) = 0;
            }
        }
    }
}

void Automaton::setLocalStates() {
    for (ul r = 0; r < state.gridSize; ++r) {
        for (ul c = 0; c < state.gridSize; ++c) {
            if (state.getW(r, c)) {
                // Check if site should die:
                // -- too acidic, or
                // -- not enough nutrients for minimal metabolism,
                // -- completed cell cycle but not divided yet.
                if (state.GI(r, c) >= params.GIdeath
                    || state.CHO(r, c) < params.metabolism.aerobicQuiescence.CHO
                    || (state.proliferationTime(r, c) > cycles.G1time(r, c)
                                                        + cycles.Stime(r, c) + cycles.G2time(r, c)
                                                        + cycles.Mtime(r, c) + cycles.Dtime(r, c))) {

                    KillSite(r, c);
                    continue;
                }

                // Check acid conditions
                if ((state.GI(r, c) < params.GIcritical)
                    // ... and whether the site has enough neighboring vacant sites,
                    // if it is near an S-phase.
                    && !(
                        state.cellCycle(r, c) == State::CellCycle::G1
                        && (state.proliferationTime(r, c)
                            >= cycles.G1time(r, c) - 2 * params.stepTime / 3600)
                        && state.proliferationTime(r, c) < cycles.G1time(r, c)
                        && vacantNeighbors(r, c).size() <= 1
                )) {
                    // [matlab] try prolif --> 2B
                    if (state.OX(r, c) >= params.metabolism.aerobicProliferation.OX) {
                        if (state.CHO(r, c) >= params.metabolism.aerobicProliferation.CHO) {
                            state.cellState(r, c) = State::CellState::AEROBIC_PROLIFERATION;
                            continue;
                        } else {  // [matlab] ix_q_a
                            // TODO moze zamiast sprawdzania tylu rzeczy wgl od razu to ustawic
                            // kiedy jest cycle zmieniany, w jakiejs osobnej fladze w state?
                            // (jeszcze pozniej pojawia sie ten warunek)
                            if (state.cellCycle(r, c) == State::CellCycle::S
                                || state.cellCycle(r, c) == State::CellCycle::M
                                || state.cellCycle(r, c) == State::CellCycle::D) {

                                KillSite(r, c);
                            } else {
                                state.cellState(r, c) = State::CellState::AEROBIC_QUIESCENCE;
                            }
                            continue;
                        }
                    } else if (state.CHO(r, c)
                               >= params.metabolism.anaerobicProliferation.CHO) {  // [matlab] ix_p_an
                        state.cellState(r, c) = State::CellState::ANAREOBIC_PROLIFERATION;
                        continue;
                    }
                }
                // [matlab] ix_try_q = ix_try_qa + ix_try_qb
                if (state.OX(r, c) >= params.metabolism.aerobicQuiescence.OX) {
                    if (state.cellCycle(r, c) == State::CellCycle::S
                        || state.cellCycle(r, c) == State::CellCycle::M
                        || state.cellCycle(r, c) == State::CellCycle::D) {

                        KillSite(r, c);
                    } else {
                        state.cellState(r, c) = State::CellState::AEROBIC_QUIESCENCE;
                    }
                    continue;
                } else {  // [matlab] ix_q_ant
                    if (state.CHO(r, c) >= params.metabolism.anaerobicQuiescence.CHO) { // [matlab] ix_q_an
                        if (state.cellCycle(r, c) == State::CellCycle::S
                            || state.cellCycle(r, c) == State::CellCycle::M
                            || state.cellCycle(r, c) == State::CellCycle::D) {

                            KillSite(r, c);
                        } else {
                            state.cellState(r, c) = State::CellState::ANAREOBIC_QUIESCENCE;
                        }
                        continue;
                    } else {  // ix_dead_b
                        KillSite(r, c);
                        continue;
                    }
                }

            }
        }
    }
}

// Progress clock if repair time is 0.
void Automaton::progressCellClock(ul r, ul c) {
    if (state.timeInRepair(r, c) == 0)
        state.proliferationTime(r, c) += params.stepTime / 3600;
}

void Automaton::metaboliseAerobicProliferation(ul r, ul c) {
    state.CHO(r, c) -= params.metabolism.aerobicProliferation.CHO;
    state.OX(r, c) -= params.metabolism.aerobicProliferation.OX;
    state.GI(r, c) += params.metabolism.aerobicProliferation.GI;
}

void Automaton::metaboliseAnaerobicProliferation(ul r, ul c) {
    state.CHO(r, c) -= params.metabolism.anaerobicProliferation.CHO;
    state.OX(r, c) -= params.metabolism.anaerobicProliferation.OX;
    state.GI(r, c) += params.metabolism.anaerobicProliferation.GI;
}

void Automaton::metaboliseAerobicQuiescence(ul r, ul c) {
    state.CHO(r, c) -= params.metabolism.aerobicQuiescence.CHO;
    state.OX(r, c) -= params.metabolism.aerobicQuiescence.OX;
    state.GI(r, c) += params.metabolism.aerobicQuiescence.GI;
}

void Automaton::metaboliseAnaerobicQuiescence(ul r, ul c) {
    state.CHO(r, c) -= params.metabolism.anaerobicQuiescence.CHO;
    state.OX(r, c) -= params.metabolism.anaerobicQuiescence.OX;
    state.GI(r, c) += params.metabolism.anaerobicQuiescence.GI;
}


void Automaton::metaboliseNutrients() {
    for (ul r = 0; r < state.gridSize; ++r) {
        for (ul c = 0; c < state.gridSize; ++c) {
            switch (state.cellState(r, c)) {
                case State::CellState::AEROBIC_PROLIFERATION:
                    progressCellClock(r, c);
                    metaboliseAerobicProliferation(r, c);
                    break;
                case State::CellState::ANAREOBIC_PROLIFERATION:
                    progressCellClock(r, c);
                    metaboliseAnaerobicProliferation(r, c);
                    break;
                case State::CellState::AEROBIC_QUIESCENCE:
                    metaboliseAerobicQuiescence(r, c);
                    break;
                case State::CellState::ANAREOBIC_QUIESCENCE:
                    metaboliseAnaerobicQuiescence(r, c);
                    break;
                default:
                    break;
            }
        }
    }
}

void Automaton::setGlobalStates() {
    for (ul r = 0; r < state.gridSize; ++r) {
        for (ul c = 0; c < state.gridSize; ++c) {
            if (state.getW(r, c)) {
                state.setCycleChanged(r, c, false);
                if (state.proliferationTime(r, c) <= cycles.G1time(r, c)
                    && state.cellCycle(r, c) != State::CellCycle::G1) {
                    state.cellCycle(r, c) = State::CellCycle::G1;
                    state.setCycleChanged(r, c, true);
                } else if (state.proliferationTime(r, c)
                           <= cycles.G1time(r, c) + cycles.Stime(r, c)
                           && state.cellCycle(r, c) != State::CellCycle::S) {
                    state.cellCycle(r, c) = State::CellCycle::S;
                    state.setCycleChanged(r, c, true);
                } else if (state.proliferationTime(r, c)
                           <= cycles.G1time(r, c) + cycles.Stime(r, c) + cycles.G2time(r, c)
                           && state.cellCycle(r, c) != State::CellCycle::G2) {
                    state.cellCycle(r, c) = State::CellCycle::G2;
                    state.setCycleChanged(r, c, true);
                } else if (state.proliferationTime(r, c) <=
                           cycles.G1time(r, c) + cycles.Stime(r, c) + cycles.G2time(r, c) + cycles.Mtime(r, c)
                           && state.cellCycle(r, c) != State::CellCycle::M) {
                    state.cellCycle(r, c) = State::CellCycle::M;
                    state.setCycleChanged(r, c, true);
                } else {
                    state.cellCycle(r, c) = State::CellCycle::D;
                }
            }
        }
    }
}

void Automaton::repairCells() {
    for (ul r = 0; r < state.gridSize; ++r) {
        for (ul c = 0; c < state.gridSize; ++c) {
            if ((state.cycleChanged(r, c) && (state.irradiation(r, c) > 0))
                || (state.timeInRepair(r, c) > 0)) {
                // add repair time to these sites
                // this ensures that any site in
                // repair mode has RepT > 0
                state.timeInRepair(r, c) += params.stepTime / 3600; // t in hours
                // DelayTime
                // provides a delay (in h) to the cell-phase cycle for
                // EMT6/Ro cells irradiated to level R (Gy) by some protocol.
                // find any sites which have come to the end of the repair period
                if (state.timeInRepair(r, c) >=
                    3.3414 * exp(0.1492 * state.irradiation(r, c))) {
                    if (state.irradiation(r, c) > 0) {
                        // TODO: put this in a different class/object
                        std::random_device rd;  //Will be used to obtain a seed for the random number engine
                        std::mt19937 rng(rd()); //Standard mersenne_twister_engine seeded with rd()
                        std::uniform_real_distribution<> dis(0.0, 1.0);
                        double rand = dis(rng);
                        if (rand <= 1 - exp(-0.4993 * state.irradiation(r, c))) {
                            KillSite(r, c);
                        } else {
                            // Repair cells that weren't killed
                            state.irradiation(r, c) = 0;
                            state.timeInRepair(r, c) = 0;
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
    for (ul r = 0; r < state.gridSize; ++r) {
        for (ul c = 0; c < state.gridSize; ++c) {
            if (isReadyForDivision(r, c)) {
                readyCells.emplace_back(r, c);
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
    auto c_r = child.first;
    auto c_c = child.second;
    cycles.G1time(c_r, c_c) = randomEngine->normal(params.birthParams.G1time.mean, params.birthParams.G1time.stddev);
    cycles.Stime(c_r, c_c) = randomEngine->normal(params.birthParams.Stime.mean, params.birthParams.Stime.stddev);
    cycles.G1time(c_r, c_c) = randomEngine->normal(params.birthParams.G2time.mean, params.birthParams.G2time.stddev);
    cycles.Mtime(c_r, c_c) = randomEngine->normal(params.birthParams.Mtime.mean, params.birthParams.Mtime.stddev);
    cycles.Dtime(c_r, c_c) = randomEngine->normal(params.birthParams.Dtime.mean, params.birthParams.Dtime.stddev);

    state.setW(c_r, c_c, true);
    state.proliferationTime(c_r, c_c) = 0;
    state.cellCycle(c_r, c_c) = State::CellCycle::G1;

    state.proliferationTime(parent.first, parent.second) = 0;
    state.cellCycle(parent.first, parent.second) = State::CellCycle::G1;

    state.irradiation(c_r, c_c) = state.irradiation(parent.first, parent.second);
}

Automaton::coords_t Automaton::randomNeighbour(ul r, ul c) {
    auto vn = vacantNeighbors(r, c);
    if (vn.empty())
        return {r, c};
    std::vector<float> probs(vn.size());
    std::transform(vn.begin(), vn.end(), probs.begin(), mapToProb);
    ul choice = randomEngine->roulette(probs);
    if (choice == probs.size()) {
        return {r, c};
    } else {
        auto relativeCoords = vn[choice];
        return {r + relativeCoords.first, c + relativeCoords.second};
    }
}

void Automaton::updateStats() {
    // TODO
}

void Automaton::KillSite(ul r, ul c) {
    state.cellState(r, c) = State::CellState::DEAD;

    state.setW(r, c, 0);
    // state.Dage(r, c)  = 0;  // TODO unused?
    state.proliferationTime(r, c) = 0;
    state.irradiation(r, c) = 0;
    state.timeInRepair(r, c) = 0;

    // Give off waste acids.
    state.GI(r, c) += params.siGI_n;
}

bool Automaton::isReadyForDivision(ul r, ul c) {
    return state.getW(r, c) &&
           state.radius(r, c) < params.rMax * state.gridSize / 2 &&
           state.cellCycle(r, c) == State::CellCycle::D;
}

float Automaton::mapToProb(std::pair<long, long> &relativeCoords) {
    constexpr float diagonalProb = 1.f / (4.f + 4.f * 1.41421356f);
    constexpr float sideProb = 1.41421356f * diagonalProb;
    if (relativeCoords.first == 0 || relativeCoords.second == 0) return sideProb;
    else return diagonalProb;
}

std::vector<std::pair<long, long>> Automaton::vacantNeighbors(ul r, ul c) {
    long r_start = r == 0 ? 0 : -1;
    long r_end = (r == state.gridSize - 1) ? 0 : 1;
    long c_start = c == 0 ? 0 : -1;
    long c_end = (c == state.gridSize - 1) ? 0 : 1;

    std::vector<std::pair<long, long>> result;
    for (long rx = r_start; rx < r_end; ++rx) {
        for (long cx = c_start; cx < c_end; ++cx) {
            if (state.getW(r + rx, c + cx) == 0) {
                result.emplace_back(rx, cx);
            }
        }
    }
    return result;
}

Automaton Automaton::loadFromFile(const std::string &filename, RandomEngine *re) {
    std::ifstream ifs(filename);
    nlohmann::json j = nlohmann::json::parse(ifs);
    State state(j);
    Cycles cycles(j);
    Parameters parameters(j);
    ul step = j["st"];
    return Automaton(state, cycles, parameters, re, step);
}

void Automaton::saveStateToFile(const std::string &filename) {
    std::ofstream ofs(filename);
    ofs << step << std::endl;
    ofs << state;
    ofs << cycles;
}
