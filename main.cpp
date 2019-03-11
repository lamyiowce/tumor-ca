#include <iostream>
#include <Automaton.h>
#include <RandomEngine.h>
#include <util.h>

int main() {
    StdRandomEngine sre(10009);
    auto ca = Automaton::loadFromFile("../tests/resources/matlab_results/out-vnw-tr1-st0-0a-initial.json", &sre);
//    ca.runNSteps(4800);
    std::cout << std::vector<int>{1, 2, 3, 4, 5} << std::endl
     << std::vector<uint8_t>{1, 0, 1, 0, 0, 0, 1} << std::endl;
    return 0;
}
