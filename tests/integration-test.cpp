#include <iostream>
#include <Automaton.h>
#include <RandomEngine.h>
#include "StateHelper.h"

int main() {
    MatlabRandomEngine sre(1);
    auto ca1 = Automaton::loadFromFile(
            "../tests/resources/matlab_results/integration/out-vnw-tr1-st0-0a-initial-integration.json",
            &sre);
    auto ca2 = Automaton::loadFromFile(
            "../tests/resources/matlab_results/integration/out-vnw-tr1-st4800-0a-result-integration.json",
            nullptr);
    ca1.runNSteps(4800);

    std::cout << "Living cells:\nsimulation:" << countLiving(ca1.getState()) << " \ncorrect: " << countLiving(ca2.getState()) << std::endl;
    bool de = discreteEquality(ca1.getState(), ca2.getState());
    std::cout << "Max numerical error:" << maxError(ca1.getState(), ca2.getState()) << std::endl;
    return !de || maxError(ca1.getState(), ca2.getState()) > MAX_RELATIVE_ERROR;
}
