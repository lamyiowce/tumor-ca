#include <catch.h>
#include <Automaton.h>
#include "StateHelper.h"

TEST_CASE("SetGlobalStates - basic test") {
    MatlabRandomEngine mre(1);
    char filepath[MAX_TEST_FILEPATH_LENGTH];
    for (const std::string& dataDir : DATA_IDS) {
        for (int i = 1; i <= N_TEST_STEPS; i++) {
            SECTION("Iteration " + std::to_string(i)) {
                sprintf(filepath,
                        "../tests/resources/matlab_results/per-function/%s/%d/out-vnw-tr1-st%d-4-MetaboliseNutrients.json",
                        dataDir.data(), i, i);
                auto ca1 = Automaton::loadFromFile(filepath, &mre);
                sprintf(filepath,
                        "../tests/resources/matlab_results/per-function/%s/%d/out-vnw-tr1-st%d-5-SetGlobalStates.json",
                        dataDir.data(), i, i);
                auto ca2 = Automaton::loadFromFile(filepath, nullptr);

                ca1.setGlobalStates();

                requireEqual(ca1, ca2);
            }
        }
    }
}
