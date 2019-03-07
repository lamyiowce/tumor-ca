#include <iostream>
#include <Cycles.h>
#include "StateHelper.h"

double maxError(const State &lhs, const State &rhs) {
    double maxErr = 0.0;
    for (ul r = 0; r < lhs.gridSize; ++r) {
        for (ul c = 0; c < lhs.gridSize; ++c) {
//            maxErr = std::max(maxErr, std::abs((lhs.CHO(r, c) - rhs.CHO(r, c))/rhs.CHO(r, c)));
            maxErr = std::max(maxErr, std::abs((lhs.OX(r, c) - rhs.OX(r, c))/rhs.OX(r, c)));
//            maxErr = std::max(maxErr, std::abs((lhs.GI(r, c) - rhs.GI(r, c))/rhs.GI(r, c)));
//            maxErr = std::max(maxErr, std::abs((lhs.timeInRepair(r, c) - rhs.timeInRepair(r, c))/rhs.timeInRepair(r, c)));
//            maxErr = std::max(maxErr, std::abs((lhs.irradiation(r, c) - rhs.irradiation(r, c))/rhs.irradiation(r, c)));
//            maxErr = std::max(maxErr, std::abs((lhs.proliferationTime(r, c) - rhs.proliferationTime(r, c))/rhs.proliferationTime(r, c)));
        }
    }
    return maxErr;
}

double maxErrorCycles(const Cycles &lhs, const Cycles &rhs) {
    double maxErr = 0.0;
    for (ul r = 0; r < lhs.gridSize; ++r) {
        for (ul c = 0; c < lhs.gridSize; ++c) {
            maxErr = std::max(maxErr, std::abs((lhs.G1time(r, c) - rhs.G1time(r, c))/rhs.G1time(r, c)));
            maxErr = std::max(maxErr, std::abs((lhs.Stime(r, c) - rhs.Stime(r, c))/rhs.Stime(r, c)));
            maxErr = std::max(maxErr, std::abs((lhs.G2time(r, c) - rhs.G2time(r, c))/rhs.G2time(r, c)));
            maxErr = std::max(maxErr, std::abs((lhs.Mtime(r, c) - rhs.Mtime(r, c))/rhs.Mtime(r, c)));
            maxErr = std::max(maxErr, std::abs((lhs.Dtime(r, c) - rhs.Dtime(r, c))/rhs.Dtime(r, c)));
        }
    }
    return maxErr;
}

bool discreteEquality(const State &lhs, const State &rhs) {
    bool res = true;
    if (lhs.gridSize != rhs.gridSize) return false;
    for (ul r = 0; r < lhs.gridSize; ++r) {
        for (ul c = 0; c < lhs.gridSize; ++c) {
            if (lhs.W(r, c) != rhs.W(r, c) ||
                lhs.cellCycle(r, c) != rhs.cellCycle(r, c) ||
                lhs.cellState(r, c) != rhs.cellState(r, c)) {
//                std::cout << r << " " << c << std::endl;
                res = false;
            }
        }
    }
    return res;
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
