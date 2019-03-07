#include <iostream>
#include <Automaton.h>
#include <RandomEngine.h>

int main() {
    MatlabRandomEngine sre(1);
    auto ca = Automaton::loadFromFile("../tests/resources/matlab_results/1/out-vnw-tr1-st1-3-SetLocalStates.json", &sre);
    auto ca0 = Automaton::loadFromFile("../tests/resources/matlab_results/1/out-vnw-tr1-st1-3-SetLocalStates.json", &sre);
    ca.metaboliseNutrients();
    auto lhs = ca.getState();
    auto ca2 = Automaton::loadFromFile("../tests/resources/matlab_results/1/out-vnw-tr1-st1-4-MetaboliseNutrients.json", &sre);
    std::cout.precision(100);
    auto rhs = ca2.getState();
    for (ul r = 0; r < lhs.gridSize; ++r) {
        for (ul c = 0; c < lhs.gridSize; ++c) {
            double err = std::abs((lhs.OX(r, c) - rhs.OX(r, c))/rhs.OX(r, c));
            if (err > 1.0E-8)
                std::cout << r << ", " << c << ": " << (int) lhs.cellState(r, c) << " " << err
                    << " V: " << lhs.OX(r, c) << " " << rhs.OX(r, c) << " " << ca0.getState().OX(r, c) << " d: " << lhs.OX(r, c) - ca0.getState().OX(r, c) <<
                     " bez: " << lhs.OX(r, c) - rhs.OX(r, c ) << " c a c' " << ca2.getState().OX(r, c) - ca0.getState().OX(r, c) << std::endl;

        }
    }
    return 0;
}
