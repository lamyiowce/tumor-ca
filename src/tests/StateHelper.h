#ifndef TUMOR_STATEHELPER_H
#define TUMOR_STATEHELPER_H

#include <State.h>

constexpr int N_TEST_STEPS = 350;
constexpr int MAX_TEST_FILEPATH_LENGTH = 512;
constexpr int FULL_N_STEPS = 144000;
constexpr double MAX_RELATIVE_ERROR = 0;
static std::string DATA_IDS[] = {/*"ONCE-20GY", */"FIVE-2GY"};

void requireEqual(const Automaton &result, const Automaton &target);

void checkEqual(const Automaton &result, const Automaton &target);

void requireEqualWithEps(const Automaton &result, const Automaton &target, double eps);

double maxError(const State &result, const State &target);

bool discreteEquality(const State &lhs, const State &rhs);

int discreteDiffCount(const State &lhs, const State &rhs);

double maxErrorCycles(const Cycles &result, const Cycles &target);

ul countLiving(const State &state);

#endif //TUMOR_STATEHELPER_H
