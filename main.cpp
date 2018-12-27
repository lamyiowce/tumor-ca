#include <iostream>
#include <Automaton.h>
#include <RandomEngine.h>
#include <memory>
#include <fstream>

int main() {
    StdRandomEngine sre(100009);
    auto ca = Automaton::loadFromFile("../tests/resources/matlab_results/out-vnw-tr1-st0-0a-initial.json", &sre);
    ca.runNSteps(50000);
    return 0;
}
