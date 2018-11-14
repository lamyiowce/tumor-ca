#include <iostream>
#include <Automaton.h>

int main() {
    unsigned long gridSize = 51;
    Parameters::Metabolism metParameters(
            Parameters::NutrientsParameters(52e-17, 0., 104e-17), // anaerobic proliferation
            Parameters::NutrientsParameters(18e-17, 8.3e-17, 1e-5), // aerobic proliferation
            Parameters::NutrientsParameters(43e-17, 0, 86e-17), // anaerobic quiescence
            Parameters::NutrientsParameters(15e-17, 5.5e-17, 0.5e-6) // aerobic quiescence
        );
    Parameters parameters(0.25, 0.25, {{0, 1}, {2, 3.0}, {3, 0.3}, {4, 0.5}}, 0.8, 0.5, metParameters, 0.95);
    State state(gridSize);
    Automaton automaton(state, parameters);
    automaton.runNSteps(10);
    std::cout << "Hello, World!" << std::endl;
    return 0;
}