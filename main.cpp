#include <iostream>
#include <Automaton.h>
#include <RandomEngine.h>
#include <memory>
#include <fstream>

int main() {
    MatlabRandomEngine mre(1);
    auto ca = Automaton::loadFromFile("../tests/resources/matlab_results/1/out-vnw-tr1-st1-3-SetLocalStates.json", &mre);
    auto ca2 = Automaton::loadFromFile("../tests/resources/matlab_results/1/out-vnw-tr1-st1-4-MetaboliseNutrients.json", nullptr);
    ca.metaboliseNutrients();
    for (ul r = 0; r < 51; ++r) {
        for (ul c = 0; c < 51; ++c) {
            std::cout << ca.getState().proliferationTime(r, c) - ca2.getState().proliferationTime(r, c) << " ";
        }
        std::cout << std::endl;
    }
    return 0;
}
