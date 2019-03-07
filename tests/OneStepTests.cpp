#include <catch.h>
#include <Automaton.h>
#include "StateHelper.h"

TEST_CASE("One step test") {
    MatlabRandomEngine mre(1);
    char filepath[MAX_TEST_FILEPATH_LENGTH];
    for (int i = 1; i <= N_TEST_STEPS - 1; i++) {
        SECTION("Iteration " + std::to_string(i)) {
            sprintf(filepath, "../tests/resources/matlab_results/%d/out-vnw-tr1-st%d-9-FinishCheck.json", i, i);
            auto ca1 = Automaton::loadFromFile(filepath, &mre);
            sprintf(filepath, "../tests/resources/matlab_results/%d/out-vnw-tr1-st%d-7-CellDivision.json", i+1, i+1);
            auto ca2 = Automaton::loadFromFile(filepath, nullptr);
            ca1.diffusion();
            ca1.irradiateTumor();
            ca1.setLocalStates();
            ca1.metaboliseNutrients();
            ca1.setGlobalStates();
            ca1.repairCells();
            ca1.cellDivision();

            REQUIRE(discreteEquality(ca1.getState(), ca2.getState()));
            REQUIRE(maxError(ca1.getState(), ca2.getState()) < MAX_RELATIVE_ERROR);
            REQUIRE(maxErrorCycles(ca1.getCycles(), ca2.getCycles()) < MAX_RELATIVE_ERROR);
        }
    }
}
