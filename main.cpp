#include <iostream>
#include <Automaton.h>
#include <RandomEngine.h>
#include <memory>

int main() {
    unsigned long gridSize = 51;
    Parameters::Metabolism metParameters(
            Parameters::NutrientsParameters(52e-17, 0., 104e-17), // anaerobic proliferation
            Parameters::NutrientsParameters(18e-17, 8.3e-17, 1e-5), // aerobic proliferation
            Parameters::NutrientsParameters(43e-17, 0, 86e-17), // anaerobic quiescence
            Parameters::NutrientsParameters(15e-17, 5.5e-17, 0.5e-6) // aerobic quiescence
    );
    Parameters::BirthParams birthParams(
            Parameters::NormDistParams(0.1, 0.1),
            Parameters::NormDistParams(0.1, 0.1),
            Parameters::NormDistParams(0.1, 0.1),
            Parameters::NormDistParams(0.1, 0.1),
            Parameters::NormDistParams(0.1, 0.1)
            );
    Parameters parameters(0.25, 0.25, {{0, 1}, {2, 3.0}, {3, 0.3}, {4, 0.5}}, 0.8, 0,
                          0.00000000000000001990000000000001,
                          0.00000000000000005000000000000001,
                          0, metParameters, 0.95, birthParams);
    State state(gridSize);
    Cycles cycles(gridSize);
    StdRandomEngine sre(167);
    Automaton automaton(state, cycles, parameters, &sre);
    automaton.runNSteps(10000);
    return 0;
}