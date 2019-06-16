#include <catch.h>
#include <Automaton.h>
#include "StateHelper.h"

TEST_CASE("Diffusion - basic test") {
    MatlabRandomEngine mre(1);
    char filepath[MAX_TEST_FILEPATH_LENGTH];
    for (const std::string& dataDir : DATA_IDS) {
        for (int i = 1; i <= N_TEST_STEPS - 1; i++) {
            SECTION("Iteration " + std::to_string(i)) {
                sprintf(filepath,
                        "../tests/resources/matlab_results/per-function/%s/%d/out-vnw-tr1-st%d-9-FinishCheck.json",
                        dataDir.data(), i, i);
                auto ca1 = Automaton::loadFromFile(filepath, &mre);
                sprintf(filepath,
                        "../tests/resources/matlab_results/per-function/%s/%d/out-vnw-tr1-st%d-1-Diffusion.json",
                        dataDir.data(), i + 1, i + 1);
                auto ca2 = Automaton::loadFromFile(filepath, nullptr);
                ca1.diffusion();

                requireEqual(ca1, ca2);
            }
        }
    }
}