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
//
//    auto ca1 = Automaton::loadFromFile("../tests/resources/matlab_results/8/out-vnw-tr1-st8-9-FinishCheck.json", &sre);
//    ca1.diffusion();
//
//    auto ca3 = Automaton::loadFromFile(
//            "../tests/resources/matlab_results/9/out-vnw-tr1-st9-1-Diffusion.json",
//            nullptr);
//    std::cout << "3de: " << discreteEquality(ca1.getState(), ca3.getState())
//              << " error: " << maxError(ca1.getState(), ca3.getState()) << std::endl;
//    ca1.irradiateTumor();
//    auto ca4 = Automaton::loadFromFile(
//            "../tests/resources/matlab_results/9/out-vnw-tr1-st9-2-IrradiateTumor.json",
//            nullptr);
//    std::cout << "4de: " << discreteEquality(ca1.getState(), ca4.getState())
//              << " error: " << maxError(ca1.getState(), ca4.getState()) << std::endl;
//
//    ca1.setLocalStates();
//    auto ca5 = Automaton::loadFromFile(
//            "../tests/resources/matlab_results/9/out-vnw-tr1-st9-3-SetLocalStates.json",
//            nullptr);
//    std::cout << "5de: " << discreteEquality(ca1.getState(), ca5.getState())
//              << " error: " << maxError(ca1.getState(), ca5.getState()) << std::endl;
//
//    ca1.metaboliseNutrients();
//    auto ca6 = Automaton::loadFromFile(
//            "../tests/resources/matlab_results/9/out-vnw-tr1-st9-4-MetaboliseNutrients.json",
//            nullptr);
//    std::cout << "6de: " << discreteEquality(ca1.getState(), ca6.getState())
//              << " error: " << maxError(ca1.getState(), ca6.getState()) << std::endl;
//
//    ca1.setGlobalStates();
//    auto ca7 = Automaton::loadFromFile(
//            "../tests/resources/matlab_results/9/out-vnw-tr1-st9-5-SetGlobalStates.json",
//            nullptr);
//    std::cout << "7de: " << discreteEquality(ca1.getState(), ca7.getState())
//              << " error: " << maxError(ca1.getState(), ca7.getState()) << std::endl;
//    ca1.repairCells();
//    auto ca8 = Automaton::loadFromFile(
//            "../tests/resources/matlab_results/9/out-vnw-tr1-st9-6-RepairCells.json",
//            nullptr);
//    std::cout << "8de: " << discreteEquality(ca1.getState(), ca8.getState())
//              << " error: " << maxError(ca1.getState(), ca8.getState()) << std::endl;
//    ca1.cellDivision();
//    auto ca9 = Automaton::loadFromFile(
//            "../tests/resources/matlab_results/9/out-vnw-tr1-st9-7-CellDivision.json",
//            nullptr);
//    std::cout << "9de: " << discreteEquality(ca1.getState(), ca9.getState())
//              << " error: " << maxError(ca1.getState(), ca9.getState()) << std::endl;
//
//
//
//    return 0;
    std::cout << countLiving(ca1.getState()) << " : " << countLiving(ca2.getState()) << std::endl;
    bool de = discreteEquality(ca1.getState(), ca2.getState());
    std::cout << de << std::endl;
    return !de || maxError(ca1.getState(), ca2.getState()) > MAX_RELATIVE_ERROR;
}
