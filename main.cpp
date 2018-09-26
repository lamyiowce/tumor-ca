#include <iostream>
#include "Automaton.h"

int main() {
    unsigned long gridSize = 51;
    Parameters parameters(0.25, 0.25, {1, 0, 3.0, 0.3, 0.5, 0}, 0.8);
    State state(gridSize);
    auto automaton = Automaton(state, parameters);
    automaton.runNSteps(10);
    std::cout << "Hello, World!" << std::endl;
    return 0;
}