#include <iostream>
#include <Automaton.h>
#include <RandomEngine.h>

int main() {
    MatlabRandomEngine sre(1);
    auto ca = Automaton::loadFromFile("../tests/resources/matlab_results/1/out-vnw-tr1-st1-5-SetGlobalStates.json", &sre);
    ca.repairCells();
    ca.cellDivision();
    auto lhs = ca.getCycles();
    auto ca2 = Automaton::loadFromFile("../tests/resources/matlab_results/1/out-vnw-tr1-st1-7-CellDivision.json", &sre);

    auto rhs = ca2.getCycles();
    for (ul r = 0; r < lhs.gridSize; ++r) {
        for (ul c = 0; c < lhs.gridSize; ++c) {
            double err = std::abs((lhs.G1time(r, c) - rhs.G1time(r, c))/rhs.G1time(r, c));
            if (err > 0.0000006)
                std::cout << r << ", " << c << " err " << err << std::endl;
        }
    }
    return 0;
}
