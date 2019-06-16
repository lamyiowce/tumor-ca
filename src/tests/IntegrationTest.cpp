#include <iostream>
#include <Automaton.h>
#include <RandomEngine.h>
#include <catch.h>
#include "StateHelper.h"

TEST_CASE("long test") {
    for (const std::string &dataDir : DATA_IDS) {
        SECTION ("144000 steps " + dataDir) {
            MatlabRandomEngine mre(1);
            auto ca1 = Automaton::loadFromFile(
                "../tests/resources/matlab_results/integration/" + dataDir + "/out-vnw-tr1-st0.json",
                &mre);

            // To run ReplenishSubstrates
            ca1.runNSteps(0);

            int i = 0;
            int checkInterval = 1000;
            std::ofstream errFile(dataDir + "_error_log.csv");
            errFile << "step,max_error,max_error_cycles,discrete_diff_count" << std::endl;

            double totalMaxErr = 0;
            double totalMaxErrCycles = 0;
            int totalDiffCount = 0;

            while (i < FULL_N_STEPS) {
                for (int j = 0; j < checkInterval; j++) {
                    ca1.advance();
                    i++;
                }
                auto ca2 = Automaton::loadFromFile(
                    "../tests/resources/matlab_results/integration/" + dataDir + "/out-vnw-tr1-st" + std::to_string(i) +
                    ".json",
                    nullptr);

                auto maxErr = maxError(ca1.getState(), ca2.getState());
                auto maxErrCycles = maxErrorCycles(ca1.getCycles(), ca2.getCycles());
                auto diffCount = discreteDiffCount(ca1.getState(), ca2.getState());

                totalMaxErr = std::max(totalMaxErr, maxErr);
                totalMaxErrCycles = std::max(totalMaxErrCycles, maxErrCycles);
                totalDiffCount = std::max(totalDiffCount, diffCount);

                errFile << i << ',' << maxErr << ',' << maxErrCycles << ',' << diffCount << std::endl;

                checkEqual(ca1, ca2);
            }

            std::cout << "Max error: " << totalMaxErr << std::endl;
            std::cout << "Max error in cycles: " << totalMaxErrCycles << std::endl;
            std::cout << "Max discrete differ count: " << totalDiffCount << std::endl;
        }
    }
}

