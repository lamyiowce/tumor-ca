#include <iostream>
#include <chrono>
#include <Automaton.h>

const char *FILE_NAME = "../tests/resources/matlab_results/integration/out-vnw-tr1-st0-0a-initial-integration.json";
const size_t ITERATIONS = 20;

int main() {
    StdRandomEngine sre(10009);
    std::chrono::milliseconds sum{0};
    for (size_t i = 0; i < ITERATIONS; ++i) {
        auto automaton = Automaton::loadFromFile(FILE_NAME, &sre);
        auto time = std::chrono::system_clock::now();
        automaton.runNSteps(4800);
        sum += std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - time);
    }
    std::cout << sum.count() / ITERATIONS << std::endl;
}