#include <iostream>
#include <Automaton.h>
#include <RandomEngine.h>

int main(int argc, char* argv[]) {
    StdRandomEngine sre(9);
    auto ca1 = Automaton::loadFromFile(
        "../tests/resources/matlab_results/integration/FIVE-2GY/out-vnw-tr1-st0.json",
        &sre);

    auto &state = ca1.state;
    for (ul r = 0; r < 51; r++) {
        for (ul c = 0; c < 51; ++c) {
            state.W(r, c) = 0;
            state.GI(r, c) = ca1.params.sGIex;
        }
    }
    ca1.replenishSubstrate();
    ca1.birthCell({25, 25}, {25, 25});

//    if (argc != 2) {
//        std::cerr << "Usage: " << argv[0] << " <nsteps>" << std::endl;
//        return 0;
//    }

    int nSteps;
//    try {
//        nSteps = std::stoi(argv[1]);
//    } catch (const std::invalid_argument& e) {
        nSteps = 10*24*601;
//    }

    ca1.runNSteps(nSteps);

    for (ul r = 0; r < 51; ++r) {
        for (ul c = 0; c < 51; ++c) {
            if (state.W(r, c))
                std::cout << (int) state.cellState(r, c) - 2<< " ";
            else
                std::cout << "· ";
//            if (state.W(r, c)) std::cout << "● "; else std::cout << "o ";
        }
        std::cout << std::endl;
    }
    std::cout << state.CHO(25, 25) << std::endl;
    return 0;
}