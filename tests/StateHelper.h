#ifndef TUMOR_STATEHELPER_H
#define TUMOR_STATEHELPER_H

#include <State.h>

const int N_TEST_STEPS = 10;
const int MAX_TEST_FILEPATH_LENGTH = 512;

double maxError(const State &lhs, const State &rhs);

bool discreteEquality(const State &lhs, const State &rhs);

#endif //TUMOR_STATEHELPER_H
