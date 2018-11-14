#include <iostream>
#include <Automaton.h>

int main() {
    unsigned long gridSize = 51;
    Parameters parameters(0.25, 0.25, {{0, 1}, {2, 3.0}, {3, 0.3}, {4, 0.5}}, 0.8, 0,
		{ 0, 0, 0, 0 },
		0.00000000000000001990000000000001,
		0.00000000000000005000000000000001,
		0);
    State state(gridSize);
	Cycles cycles(51);
    Automaton automaton(state, cycles, parameters);
    automaton.runNSteps(10);
    std::cout << "Hello, World!" << std::endl;
    return 0;
}