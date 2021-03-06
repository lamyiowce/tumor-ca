#include <Automaton.h>
#include <algorithm>
#include <iostream>
#include <math.h>
#include <fstream>

Automaton::Automaton(const State &_state, const Cycles &_cycles, const Parameters &_params,
                     RandomEngine *randomEngine, ul _step)
        : state(_state), cycles(_cycles), params(_params), step(_step), randomEngine(randomEngine) {}

const State &Automaton::getState() const {
    return state;
}

void Automaton::runNSteps(int nSteps) {
    replenishSubstrate();
    for (int i = 0; i < nSteps; i++)
        advance();
}

void Automaton::advance() {
    ++step;
    diffusion();
    irradiateTumor();
    setLocalStates();
    metaboliseNutrients();
    setGlobalStates();
    repairCells();
    cellDivision();
}

void Automaton::replenishSubstrate() {
    for (ul r = 0; r < state.gridSize; r++)
        for (ul c = 0; c < state.gridSize; c++)
            if (!state.W(r, c)) {
                state.CHO(r, c) = params.sCHOex;
                state.OX(r, c) = params.sOXex;
            }
}

static double paramDiffusion(double_p val, double_p d, double_p tau, double_p orthoSum, double_p diagSum) {
    constexpr double_p HS = MATLAB_2SQRT2;
    constexpr double_p f = 4. + HS;
    return (d*tau*HS)/f * (orthoSum + MATLAB_1_2SQRT2*diagSum - f*val) + val;
}

std::pair<double_p, double_p> Automaton::sumNeighbors(ul r, ul c, const grid<double_p> &values, ul gridW) {
    double_p orthogonalResult = 0.0, diagonalResult = 0.0;
    orthogonalResult += values[(r-1) * gridW + c];
    orthogonalResult += values[(r+1) * gridW + c];
    orthogonalResult += values[r * gridW + c-1];
    orthogonalResult += values[r * gridW + c+1];

    diagonalResult += values[(r-1) * gridW + c-1];
    diagonalResult += values[(r+1) * gridW + c-1];
    diagonalResult += values[(r+1) * gridW + c+1];
    diagonalResult += values[(r-1) * gridW + c+1];
    return {orthogonalResult, diagonalResult};
}

void Automaton::numericalDiffusion(ul r, ul c, const grid<double_p> &choCopy, const grid<double_p> &oxCopy,
                                   const grid<double_p> &giCopy, grid<double_p> &choResult, grid<double_p> &oxResult,
                                   grid<double_p> &giResult, ul gridW) {
    auto index = r * gridW + c;
    auto paramSums = sumNeighbors(r, c, choCopy, gridW);
    choResult[index] = paramDiffusion(choCopy[index], params.sDCHO, params.tau, paramSums.first, paramSums.second);
    paramSums = sumNeighbors(r, c, oxCopy, gridW);
    oxResult[index] = paramDiffusion(oxCopy[index], params.sDOX, params.tau, paramSums.first, paramSums.second);
    paramSums = sumNeighbors(r, c, giCopy, gridW);
    giResult[index] = paramDiffusion(giCopy[index], params.sDGI, params.tau, paramSums.first, paramSums.second);
}

static double_p distance(std::pair<double_p, double_p> p1, std::pair<double_p, double_p> p2) {
    return std::sqrt((p1.first - p2.first) * (p1.first - p2.first) + (p1.second - p2.second) * (p1.second - p2.second));
}

std::pair<Automaton::coords_t, ul> Automaton::findTumor() {
    ul minC = state.gridSize;
    ul minR = state.gridSize;
    ul maxC = 0;
    ul maxR = 0;
    for (ul r = 0; r < state.gridSize; ++r) {
        for (ul c = 0; c < state.gridSize; ++c) {
            if (state.W(r, c)) {
                minC = std::min(c, minC);
                minR = std::min(r, minR);
                maxC = std::max(c, maxC);
                maxR = std::max(r, maxR);
            }
        }
    }
    ul midC = minC + (ul) std::round(0.5 * (maxC - minC));
    ul midR = minR + (ul) std::round(0.5 * (maxR - minR));
    ul maxDist = 0;
    for (ul r = minR; r <= maxR; ++r) {
        for (ul c = minC; c <= maxC; ++c) {
            if (state.W(r, c)) {
                maxDist = std::max(ul(std::ceil(distance({r, c}, {midR, midC}))), maxDist);
            }
        }
    }
    return {{midR, midC}, maxDist};
}


std::pair<Automaton::coords_t, Automaton::coords_t> Automaton::findSubLattice(coords_t mid, ul maxDist) {
    ul midR, midC;
    std::tie(midR, midC) = mid;
    ul subLatticeR = (midR <= maxDist) ? 0 : midR - maxDist; // sub-lattice upper row
    ul subLatticeC = (midC <= maxDist) ? 0 : midC - maxDist; // sub-lattice left column
    ul subLatticeW =                                         // sub-lattice width
            (midC + maxDist >= state.gridSize) ? state.gridSize - subLatticeC : midC + maxDist - subLatticeC + 1;
    ul subLatticeH =                                         // sub-lattice height
            (midR + maxDist >= state.gridSize) ? state.gridSize - subLatticeR : midR + maxDist - subLatticeR + 1;
    return {{subLatticeR, subLatticeC}, {subLatticeH, subLatticeW}};
}

Automaton::LatticeCopy Automaton::copySubLattice(ul borderedH, ul borderedW, ul subLatticeR, ul subLatticeC) {
    LatticeCopy copy(borderedH, borderedW);
    auto subLatticeH = borderedH - 4;
    auto subLatticeW = borderedW - 4;
    for (ul r = 0; r < subLatticeH; ++r) {
        for (ul c = 0; c < subLatticeW; ++c) {
            copy.CHO[0][(r + 2) * borderedW + (c + 2)] = state.CHO(subLatticeR + r, subLatticeC + c);
            copy.OX[0][(r + 2) * borderedW + (c + 2)] = state.OX(subLatticeR + r, subLatticeC + c);
            copy.GI[0][(r + 2) * borderedW + (c + 2)] = state.GI(subLatticeR + r, subLatticeC + c);
        }
    }
    return copy;
}

void Automaton::calculateDiffusion(
        Automaton::LatticeCopy &copy,
        std::vector<Automaton::coords_t> borderSites,
        ul rounds) {
    auto borderedW = copy.borderedW;
    auto borderedH = copy.borderedH;
    for (ul i = 0; i < rounds; ++i) {
        for (auto rc: borderSites) {
            auto r = rc.first;
            auto c = rc.second;
            copy.CHO[i % 2][(r+1)*borderedW + c+1] = params.sCHOex;
            copy.OX[i % 2][(r+1)*borderedW + c+1] = params.sOXex;
            copy.GI[i % 2][(r+1)*borderedW + c+1] = params.sGIex;
        }
        for (ul r = 0; r < borderedH-2; ++r) {
            for (ul c = 0; c < borderedW-2; ++c) {
                numericalDiffusion(r+1, c+1, copy.CHO[i % 2], copy.OX[i % 2], copy.GI[i % 2],
                                   copy.CHO[(i + 1) % 2], copy.OX[(i + 1) % 2],
                                   copy.GI[(i + 1) % 2], borderedW);
            }
        }
    }
}

std::vector<Automaton::coords_t> Automaton::findBorderSites(ul borderedH, ul borderedW, ul maxDist) {
    std::vector<coords_t> borderSites;
    for (ul r = 0; r < borderedH-2; ++r) {
        for (ul c = 0; c < borderedW-2; ++c) {
            if (distance({r+1, c+1}, {double_p(borderedH-2) / 2., double_p(borderedW-2) / 2.}) >= maxDist) {
                borderSites.emplace_back(r, c);
            }
        }
    }
    return borderSites;
}

void Automaton::diffusion() {
    coords_t tumorMid;
    ul maxLivingDistance;
    std::tie(tumorMid, maxLivingDistance) = findTumor();
    auto subLatticeCoords = findSubLattice(tumorMid, maxLivingDistance);
    ul subLatticeR, subLatticeC, subLatticeW, subLatticeH;
    std::tie(subLatticeR, subLatticeC) = subLatticeCoords.first;
    std::tie(subLatticeH, subLatticeW) = subLatticeCoords.second;

    // add two sites wide border
    auto borderedW = subLatticeW + 4;
    auto borderedH = subLatticeH + 4;
    auto copy = copySubLattice(borderedH, borderedW, subLatticeR, subLatticeC);
    auto borderSites = findBorderSites(borderedH, borderedW, maxLivingDistance);
    ul rounds = ul(std::round(params.stepTime / params.tau));
    calculateDiffusion(copy, borderSites, rounds);
    for (ul r = 0; r < subLatticeH; ++r) {
        for (ul c = 0; c < subLatticeW; ++c) {
            state.CHO(subLatticeR + r, subLatticeC + c) = copy.CHO[rounds % 2][(r + 2) * borderedW + (c + 2)];
            state.OX(subLatticeR + r, subLatticeC + c) = copy.OX[rounds % 2][(r + 2) * borderedW + (c + 2)];
            state.GI(subLatticeR + r, subLatticeC + c) = copy.GI[rounds % 2][(r + 2) * borderedW + (c + 2)];
        }
    }
}

void Automaton::irradiateTumor() {
    double_p dose = params.irradiationSteps.getIrradiationDose(step);
    if (dose <= 0) return;
    for (ul r = 0; r < state.gridSize; ++r) {
        for (ul c = 0; c < state.gridSize; ++c) {
            if (state.W(r, c)) {
                const auto r0 = state.irradiation(r, c);
                // TODO tau change: comment / refactor
                params.tau = 0.8;
                const auto timeInRepair = state.timeInRepair(r, c);
                const auto effectiveIrradiation = r0 / (1 + timeInRepair / params.tau);
                state.irradiation(r, c) = effectiveIrradiation + dose;
                state.timeInRepair(r, c) = 0;
                params.tau = 0.25;
            }
        }
    }
}

void Automaton::setLocalStates() {
    std::vector<std::pair<ul, ul>> sitesToKill{};

    for (ul r = 0; r < state.gridSize; ++r) {
        for (ul c = 0; c < state.gridSize; ++c) {
            // Matlab model compatibility: the state is not defined for all cells.
            state.cellState(r, c) = State::CellState::EMPTY;
            if (state.W(r, c)) {
                // Check if site should die:
                // -- too acidic, or
                // -- not enough nutrients for minimal metabolism,
                // -- completed cell cycle but not divided yet.
                if (state.GI(r, c) >= params.GIdeath
                    || state.CHO(r, c) < params.metabolism.aerobicQuiescence.CHO
                    || (state.proliferationTime(r, c) > cycles.G1time(r, c)
                                                        + cycles.Stime(r, c) + cycles.G2time(r, c)
                                                        + cycles.Mtime(r, c) + cycles.Dtime(r, c))) {

                    sitesToKill.emplace_back(r, c);
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
                            if (state.cellCycle(r, c) == State::CellCycle::S
                                || state.cellCycle(r, c) == State::CellCycle::M
                                || state.cellCycle(r, c) == State::CellCycle::D) {

                                sitesToKill.emplace_back(r, c);
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

                        sitesToKill.emplace_back(r, c);
                    } else {
                        state.cellState(r, c) = State::CellState::AEROBIC_QUIESCENCE;
                    }
                    continue;
                } else {  // [matlab] ix_q_ant
                    if (state.CHO(r, c) >= params.metabolism.anaerobicQuiescence.CHO) { // [matlab] ix_q_an
                        if (state.cellCycle(r, c) == State::CellCycle::S
                            || state.cellCycle(r, c) == State::CellCycle::M
                            || state.cellCycle(r, c) == State::CellCycle::D) {

                            sitesToKill.emplace_back(r, c);
                        } else {
                            state.cellState(r, c) = State::CellState::ANAREOBIC_QUIESCENCE;
                        }
                        continue;
                    } else {  // ix_dead_b
                        sitesToKill.emplace_back(r, c);
                        continue;
                    }
                }

            }
        }
    }

    for (auto siteCoords : sitesToKill) {
        state.cellState(siteCoords.first, siteCoords.second) = State::CellState::DEAD;
        KillSite(siteCoords.first, siteCoords.second);
    }
}

// Progress clock if repair time is 0.
void Automaton::progressCellClock(ul r, ul c) {
    if (state.timeInRepair(r, c) == 0) {
        state.proliferationTime(r, c) += params.stepTime / 3600;
    }
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
            state.cycleChanged(r, c) = false;
            if (state.W(r, c)) {
                if (state.proliferationTime(r, c) <= cycles.G1time(r, c)) {
                    if (state.cellCycle(r, c) != State::CellCycle::G1) {
                        state.cellCycle(r, c) = State::CellCycle::G1;
                        state.cycleChanged(r, c) = true;
                    }
                } else if (state.proliferationTime(r, c)
                           <= cycles.G1time(r, c) + cycles.Stime(r, c)) {
                    if (state.cellCycle(r, c) != State::CellCycle::S) {
                        state.cellCycle(r, c) = State::CellCycle::S;
                        state.cycleChanged(r, c) = true;
                    }
                } else if (state.proliferationTime(r, c)
                           <= cycles.G1time(r, c) + cycles.Stime(r, c) + cycles.G2time(r, c)) {
                    if (state.cellCycle(r, c) != State::CellCycle::G2) {
                        state.cellCycle(r, c) = State::CellCycle::G2;
                        state.cycleChanged(r, c) = true;
                    }
                } else if (state.proliferationTime(r, c) <=
                           cycles.G1time(r, c) + cycles.Stime(r, c) + cycles.G2time(r, c) + cycles.Mtime(r, c)) {
                    if (state.cellCycle(r, c) != State::CellCycle::M) {
                        state.cellCycle(r, c) = State::CellCycle::M;
                        state.cycleChanged(r, c) = true;
                    }
                } else {
                    state.cellCycle(r, c) = State::CellCycle::D;
                }
            }
        }
    }
}

void Automaton::repairCells() {
    for (ul c = 0; c < state.gridSize; ++c) {
        for (ul r = 0; r < state.gridSize; ++r) {
            if ((state.cycleChanged(r, c) && (state.irradiation(r, c) > 0))
                || (state.timeInRepair(r, c) > 0)) { // ix_Rep
                state.timeInRepair(r, c) += params.stepTime / 3600;
                auto delay_time = 3.3414 * std::exp(0.1492 * state.irradiation(r, c));
                if (state.timeInRepair(r, c) >= delay_time) { // ix_FinRep
                    auto death_prob = 1 - std::exp(-0.4993 * state.irradiation(r, c));
                    if (randomEngine->uniform() <= death_prob) {
                        KillSite(r, c);
                    } else {
                        state.irradiation(r, c) = 0;
                        state.timeInRepair(r, c) = 0;
                    }
                }
            }
        }
    }
}

void Automaton::KillSite(ul r, ul c) {
    state.W(r, c) = 0;
    state.proliferationTime(r, c) = 0;
    state.irradiation(r, c) = 0;
    state.timeInRepair(r, c) = 0;

    // Give off waste acids.
    state.GI(r, c) += params.siGI_n;
}

void Automaton::cellDivision() {
    std::vector<std::pair<ul, ul>> readyCells{};
    for (ul c = 0; c < state.gridSize; ++c) {
        for (ul r = 0; r < state.gridSize; ++r) {
            if (isReadyForDivision(r, c)) {
                readyCells.emplace_back(r, c);
            }
        }
    }
    auto allReadyCellsSize = readyCells.size();
    for (ul i = 0; i < allReadyCellsSize; ++i) {
        auto permutation = randomEngine->randomPermutation(readyCells.size());
        ul n = permutation[0];
        auto cell = readyCells[n];
        auto child = randomNeighbour(cell.first, cell.second);
        if (child != cell) {
            birthCell(cell, child);
        }
        readyCells.erase(readyCells.begin() + n);
    }
}

void Automaton::birthCell(const Automaton::coords_t &parent, const Automaton::coords_t &child) {
    auto c_r = child.first;
    auto c_c = child.second;
    cycles.G1time(c_r, c_c) = randomEngine->normal(params.birthParams.G1time.mean, params.birthParams.G1time.stddev);
    cycles.Stime(c_r, c_c) = randomEngine->normal(params.birthParams.Stime.mean, params.birthParams.Stime.stddev);
    cycles.G2time(c_r, c_c) = randomEngine->normal(params.birthParams.G2time.mean, params.birthParams.G2time.stddev);
    cycles.Mtime(c_r, c_c) = randomEngine->normal(params.birthParams.Mtime.mean, params.birthParams.Mtime.stddev);
    cycles.Dtime(c_r, c_c) = randomEngine->normal(params.birthParams.Dtime.mean, params.birthParams.Dtime.stddev);

    state.W(c_r, c_c) = 1;
    state.proliferationTime(c_r, c_c) = 0;
    state.cellCycle(c_r, c_c) = State::CellCycle::G1;

    state.proliferationTime(parent.first, parent.second) = 0;
    state.cellCycle(parent.first, parent.second) = State::CellCycle::G1;

    state.irradiation(c_r, c_c) = state.irradiation(parent.first, parent.second);
}

Automaton::coords_t Automaton::randomNeighbour(ul r, ul c) {
    auto vn = vacantNeighbors(r, c);
    if (vn.empty()) {
        return {r, c};
    }
    std::vector<double> probs(vn.size());
    std::transform(vn.begin(), vn.end(), probs.begin(), mapToProb);
    ul choice = randomEngine->roulette(probs);
    if (choice == probs.size()) {
        return {r, c};
    } else {
        auto relativeCoords = vn[choice];
        return {r + relativeCoords.first, c + relativeCoords.second};
    }
}

bool Automaton::isReadyForDivision(ul r, ul c) {
    return state.W(r, c) &&
           state.radius(r, c) < params.rMax * state.gridSize / 2 &&
           state.cellCycle(r, c) == State::CellCycle::D;
}

double Automaton::mapToProb(std::pair<long, long> &relativeCoords) {
    constexpr double diagonalProb = 1 / (4 + 4 * 1.414213562373095);
    constexpr double sideProb = 1.414213562373095 * diagonalProb;
    if (relativeCoords.first == 0 || relativeCoords.second == 0) return sideProb;
    else return diagonalProb;
}

std::vector<std::pair<long, long>> Automaton::vacantNeighbors(ul r, ul c) {
    long r_start = r == 0 ? 0 : -1;
    long r_end = (r == state.gridSize - 1) ? 0 : 1;
    long c_start = c == 0 ? 0 : -1;
    long c_end = (c == state.gridSize - 1) ? 0 : 1;

    std::vector<std::pair<long, long>> result{};
    for (long cx = r_start; cx <= c_end; ++cx) {
        for (long rx = c_start; rx <= r_end; ++rx) {
            if (state.W(r + rx, c + cx) == 0) {
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

Cycles Automaton::getCycles() const{
    return cycles;
}

void Automaton::saveStateToFile(const std::string &filename) {
    std::ofstream ofs(filename);
    ofs << step << std::endl;
    ofs << state;
    ofs << cycles;
}

void Automaton::setIrradiationProtocol(IrradiationProtocol protocol) {
    params.setIrradiationProtocol(std::move(protocol));
}

void Automaton::setRandomEngine(RandomEngine *re) {
    this->randomEngine = re;
}
