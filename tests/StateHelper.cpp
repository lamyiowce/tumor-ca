#include "StateHelper.h"

double maxError(const State &lhs, const State &rhs) {
    double maxErr = 0.0;
    for (ul i = 0; i < lhs.gridSize; ++i) {
        for (ul j = 0; j < lhs.gridSize; ++j) {
            maxErr = std::max(maxErr, std::abs(lhs.CHO(i, j) - rhs.CHO(i, j)));
            maxErr = std::max(maxErr, std::abs(lhs.OX(i, j) - rhs.OX(i, j)));
            maxErr = std::max(maxErr, std::abs(lhs.GI(i, j) - rhs.GI(i, j)));
            maxErr = std::max(maxErr, std::abs(lhs.timeInRepair(i, j) - rhs.timeInRepair(i, j)));
            maxErr = std::max(maxErr, std::abs(lhs.irradiation(i, j) - rhs.irradiation(i, j)));
            maxErr = std::max(maxErr, std::abs(lhs.proliferationTime(i, j) - rhs.proliferationTime(i, j)));
        }
    }
    return maxErr;
}

bool discreetEquality(const State &lhs, const State &rhs) {
    if (lhs.gridSize != rhs.gridSize) return false;
    for (ul i = 0; i < lhs.gridSize; ++i) {
        for (ul j = 0; j < lhs.gridSize; ++j) {
            if (lhs.getW(i, j) != rhs.getW(i, j) ||
                lhs.cellCycle(i, j) != rhs.cellCycle(i, j) ||
                lhs.cellState(i, j) != rhs.cellState(i, j))
                return false;
        }
    }
    return true;
}
