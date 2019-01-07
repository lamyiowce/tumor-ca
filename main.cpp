#include <iostream>
#include <Automaton.h>
#include <RandomEngine.h>

int main() {
    MatlabRandomEngine mre(10009);
    auto ca = Automaton::loadFromFile("../tests/resources/matlab_results/out-vnw-tr1-st0-0a-initial.json", &mre);
    ca.runNSteps(4800);
    return 0;
}
