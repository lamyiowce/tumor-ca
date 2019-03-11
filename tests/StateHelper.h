#ifndef TUMOR_STATEHELPER_H
#define TUMOR_STATEHELPER_H

#include <State.h>

constexpr int N_TEST_STEPS = 10;
constexpr int MAX_TEST_FILEPATH_LENGTH = 512;

constexpr double MAX_RELATIVE_ERROR = 1e-20;

double maxError(const State &lhs, const State &rhs);

bool discreteEquality(const State &lhs, const State &rhs);

#endif //TUMOR_STATEHELPER_H
