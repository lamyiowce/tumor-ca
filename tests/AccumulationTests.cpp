#include <catch.h>
#include <Automaton.h>
#include <iostream>
#include "StateHelper.h"
/*
TEST_CASE("Error accumulation test") {
    MatlabRandomEngine mre(1);
    auto ca1 = Automaton::loadFromFile("../tests/resources/matlab_results/out-vnw-tr1-st0-0a-initial.json", &mre);
    ca1.replenishSubstrate();
    char filepath[MAX_TEST_FILEPATH_LENGTH];
    for (int i = 1; i <= N_TEST_STEPS; i++) {
        SECTION("Iteration " + std::to_string(i)) {
            sprintf(filepath, "../tests/resources/matlab_results/%d/out-vnw-tr1-st%d-7-CellDivision.json", i, i);
            auto ca2 = Automaton::loadFromFile(filepath, nullptr);
            ca1.diffusion();
            ca1.irradiateTumor();
            ca1.setLocalStates();
            ca1.metaboliseNutrients();
            ca1.setGlobalStates();
            ca1.repairCells();
            ca1.cellDivision();

//            std::cout << countLiving(ca1.getState()) << " : " << countLiving(ca2.getState()) << std::endl;
            bool de = discreteEquality(ca1.getState(), ca2.getState());
            REQUIRE(de);
            REQUIRE(maxError(ca1.getState(), ca2.getState()) < MAX_RELATIVE_ERROR);
        }
    }
}
 */