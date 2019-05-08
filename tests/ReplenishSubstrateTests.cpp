#include <catch.h>
#include <Automaton.h>
#include "StateHelper.h"

TEST_CASE("ReplenishSubstrate - basic test") {
    MatlabRandomEngine mre(1);
    auto ca1 = Automaton::loadFromFile("../tests/resources/matlab_results/per-function/out-vnw-tr1-st0-0a-initial.json", &mre);
    ca1.replenishSubstrate();
    auto ca2 = Automaton::loadFromFile(
            "../tests/resources/matlab_results/per-function/out-vnw-tr1-st0-0b-ReplenishSubstrate.json",
            nullptr);

    requireEqual(ca1, ca2);
}