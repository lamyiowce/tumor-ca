#include <catch.h>
#include <Automaton.h>
#include "StateHelper.h"

TEST_CASE("Irradiate - basic test") {
    MatlabRandomEngine mre(1);
    auto ca1 = loadAutomaton("../tests/resources/matlab_results/1/out-vnw-tr1-st1-1-Diffusion.json", &mre);
    ca1.setStep(0); // just to show it's possible
    ca1.irradiateTumor();
    auto ca2 = loadAutomaton("../tests/resources/matlab_results/1/out-vnw-tr1-st1-2-IrradiateTumor.json", nullptr);
    REQUIRE(discreetEquality(ca1.getState(), ca2.getState()));
    REQUIRE(maxError(ca1.getState(), ca2.getState()) < 0.00005);
}