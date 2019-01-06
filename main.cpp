#include <iostream>
#include <Automaton.h>
#include <RandomEngine.h>
#include <memory>
#include <fstream>

int main() {
    MatlabRandomEngine mre(1);
    auto ca = Automaton::loadFromFile("../tests/resources/matlab_results/1/out-vnw-tr1-st1-9-FinishCheck.json", &mre);
    auto ca1 = Automaton::loadFromFile("../tests/resources/matlab_results/1/out-vnw-tr1-st1-9-FinishCheck.json", &mre);
    ca.diffusion();
    auto ca2 = Automaton::loadFromFile("../tests/resources/matlab_results/2/out-vnw-tr1-st2-1-Diffusion.json", &mre);

    for (ul r = 0; r < 51; ++r) {
        for (ul c = 0; c < 51; ++c) {
//            std::cout << ((ca.getState().CHO(r, c) - ca2.getState().CHO(r, c))) << " ";
            std::cout << ((ca.getState().GI(r, c) - ca2.getState().GI(r, c)) / ca2.getState().GI(r, c)) << " ";
//            std::cout << (ca2.getState().CHO(r, c) - ca1.getState().CHO(r, c)) << " ";
        }
        std::cout << std::endl;
    }
    return 0;
}
