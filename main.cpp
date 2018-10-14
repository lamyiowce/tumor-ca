#include <iostream>
#include <Automaton.h>

int main() {
    unsigned long gridSize = 51;
    Parameters parameters(0.25, 0.25, {{0, 1}, {2, 3.0}, {3, 0.3}, {4, 0.5}}, 0.8);
    State state(gridSize);
    Automaton automaton(state, parameters);
    automaton.runNSteps(10);
    std::cout << "Hello, World!" << std::endl;
    return 0;
}