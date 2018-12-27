#include <catch.h>
#include <Automaton.h>
#include "StateHelper.h"

TEST_CASE("RepairCells & CellDivision - basic test") {
    MatlabRandomEngine mre(1);
    char filepath[MAX_TEST_FILEPATH_LENGTH];
    for (int i = 1; i <= N_TEST_STEPS; i++) {
        SECTION("Iteration " + std::to_string(i)) {
            sprintf(filepath, "../tests/resources/matlab_results/%d/out-vnw-tr1-st%d-5-SetGlobalStates.json", i, i);
            auto ca1 = Automaton::loadFromFile(filepath, &mre);
            sprintf(filepath, "../tests/resources/matlab_results/%d/out-vnw-tr1-st%d-7-CellDivision.json", i, i);
            auto ca2 = Automaton::loadFromFile(filepath, nullptr);
            ca1.repairCells();
            ca1.cellDivision();

            REQUIRE(discreteEquality(ca1.getState(), ca2.getState()));
            REQUIRE(maxError(ca1.getState(), ca2.getState()) < 0.0000000000000001);
        }
    }
}