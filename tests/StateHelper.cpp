#include "StateHelper.h"

double maxError(const State &lhs, const State &rhs) {
    double maxErr = 0.0;
    for (ul r = 0; r < lhs.gridSize; ++r) {
        for (ul c = 0; c < lhs.gridSize; ++c) {
            maxErr = std::max(maxErr, std::abs((lhs.CHO(r, c) - rhs.CHO(r, c))/rhs.CHO(r, c)));
            maxErr = std::max(maxErr, std::abs((lhs.OX(r, c) - rhs.OX(r, c))/rhs.OX(r, c)));
            maxErr = std::max(maxErr, std::abs((lhs.GI(r, c) - rhs.GI(r, c))/rhs.GI(r, c)));
            maxErr = std::max(maxErr, std::abs((lhs.timeInRepair(r, c) - rhs.timeInRepair(r, c))/rhs.timeInRepair(r, c)));
            maxErr = std::max(maxErr, std::abs((lhs.irradiation(r, c) - rhs.irradiation(r, c))/rhs.irradiation(r, c)));
            maxErr = std::max(maxErr, std::abs((lhs.proliferationTime(r, c) - rhs.proliferationTime(r, c))/rhs.proliferationTime(r, c)));
        }
    }
    return maxErr;
}

bool discreteEquality(const State &lhs, const State &rhs) {
    if (lhs.gridSize != rhs.gridSize) return false;
    for (ul r = 0; r < lhs.gridSize; ++r) {
        for (ul c = 0; c < lhs.gridSize; ++c) {
            if (lhs.getW(r, c) != rhs.getW(r, c) ||
                lhs.cellCycle(r, c) != rhs.cellCycle(r, c) ||
                lhs.cellState(r, c) != rhs.cellState(r, c))
                return false;
        }
    }
    return true;
}
