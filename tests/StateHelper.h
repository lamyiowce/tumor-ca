#ifndef TUMOR_STATEHELPER_H
#define TUMOR_STATEHELPER_H

#include <State.h>

double maxError(const State &lhs, const State &rhs);

bool discreteEquality(const State &lhs, const State &rhs);

#endif //TUMOR_STATEHELPER_H
