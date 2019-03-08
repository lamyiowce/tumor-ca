#include <iostream>
#include <Automaton.h>
#include <RandomEngine.h>

int main() {
    StdRandomEngine sre(9);
    auto ca1 = Automaton::loadFromFile(
            "../tests/resources/matlab_results/integration/out-vnw-tr1-st0-0a-initial-integration.json",
            &sre);
    ca1.runNSteps(4800);
    return 0;
}
