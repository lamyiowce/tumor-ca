#include <iostream>
#include <Automaton.h>
#include <RandomEngine.h>

int main(int argc, char* argv[]) {
    StdRandomEngine sre(9);
    auto ca1 = Automaton::loadFromFile(
        "../tests/resources/matlab_results/integration/ONCE-20GY/out-vnw-tr1-st0.json",
        &sre);

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <nsteps>" << std::endl;
        return 0;
    }

    int nSteps;
    try {
        nSteps = std::stoi(argv[1]);
    } catch (const std::invalid_argument& e) {
        std::cerr << "Error in parsing nSteps." << std::endl;
        return 1;
    }

    ca1.runNSteps(nSteps);
    return 0;
}