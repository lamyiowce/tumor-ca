#include <iostream>
#include <Cycles.h>
#include <Automaton.h>
#include <catch.h>
#include "StateHelper.h"

double maxError(const State &result, const State &target) {
    single_p maxErr = 0.0;

    for (ul r = 0; r < result.gridSize; ++r) {
        for (ul c = 0; c < result.gridSize; ++c) {
            maxErr = std::max(maxErr, std::abs((result.CHO(r, c) - target.CHO(r, c))/target.CHO(r, c)));
            maxErr = std::max(maxErr, std::abs((result.OX(r, c) - target.OX(r, c))/target.OX(r, c)));
            maxErr = std::max(maxErr, std::abs((result.GI(r, c) - target.GI(r, c))/target.GI(r, c)));
            maxErr = std::max(maxErr, std::abs((result.timeInRepair(r, c) - target.timeInRepair(r, c))/target.timeInRepair(r, c)));
            maxErr = std::max(maxErr, std::abs((result.irradiation(r, c) - target.irradiation(r, c))/target.irradiation(r, c)));
            maxErr = std::max(maxErr, std::abs((result.proliferationTime(r, c) - target.proliferationTime(r, c))/target.proliferationTime(r, c)));
        }
    }
    return maxErr;
}

double maxErrorCycles(const Cycles &result, const Cycles &target) {
    single_p maxErr = 0.0;
    for (ul r = 0; r < result._gridSize; ++r) {
        for (ul c = 0; c < result._gridSize; ++c) {
            maxErr = std::max(maxErr, std::abs((result.G1time(r, c) - target.G1time(r, c))/target.G1time(r, c)));
            maxErr = std::max(maxErr, std::abs((result.Stime(r, c) - target.Stime(r, c))/target.Stime(r, c)));
            maxErr = std::max(maxErr, std::abs((result.G2time(r, c) - target.G2time(r, c))/target.G2time(r, c)));
            maxErr = std::max(maxErr, std::abs((result.Mtime(r, c) - target.Mtime(r, c))/target.Mtime(r, c)));
            maxErr = std::max(maxErr, std::abs((result.Dtime(r, c) - target.Dtime(r, c))/target.Dtime(r, c)));
        }
    }
    return maxErr;
}

int discreteDiffCount(const State &lhs, const State &rhs) {
    int count = 0;
    assert(lhs.gridSize == rhs.gridSize);
    for (ul r = 0; r < lhs.gridSize; ++r) {
        for (ul c = 0; c < lhs.gridSize; ++c) {
            if (lhs.W(r, c) != rhs.W(r, c) ||
                lhs.cellCycle(r, c) != rhs.cellCycle(r, c) ||
                lhs.cellState(r, c) != rhs.cellState(r, c) ||
                lhs.cycleChanged(r, c) != rhs.cycleChanged(r, c)) {
                ++count;
            }
        }
    }
    return count;
}


bool discreteEquality(const State &lhs, const State &rhs) {
    if (lhs.gridSize != rhs.gridSize) return false;
    auto count = discreteDiffCount(lhs, rhs);
    return count == 0;
}

ul countLiving(const State &state) {
    ul count = 0;
    for (ul r = 0; r < state.gridSize; ++r) {
        for (ul c = 0; c < state.gridSize; ++c) {
            if (state.W(r, c)) ++count;
        }
    }
    return count;
}

void requireEqualWithEps(const Automaton &result, const Automaton &target,
                         double eps) {
    REQUIRE(discreteEquality(result.getState(), target.getState()));
    REQUIRE(maxError(result.getState(), target.getState()) <= eps);
    REQUIRE(maxErrorCycles(result.getCycles(), target.getCycles()) <= eps);
}

void requireEqual(const Automaton& result, const Automaton& target) {
    requireEqualWithEps(result, target, MAX_RELATIVE_ERROR);
}

void checkEqual(const Automaton &result, const Automaton &target) {
    CHECK(discreteEquality(result.getState(), target.getState()));
    CHECK(maxError(result.getState(), target.getState()) <= MAX_RELATIVE_ERROR);
    CHECK(maxErrorCycles(result.getCycles(), target.getCycles()) <= MAX_RELATIVE_ERROR);
}
