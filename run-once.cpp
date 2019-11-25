#include <iostream>
#include <Automaton.h>

int countLiving(const State &state) {
  int result = 0;
  for (ul r = 0; r < state.gridSize; ++r) {
    for (ul c = 0; c < state.gridSize; ++c) {
      if (state.W(r, c)) ++result;
    }
  }
  return result;
}

const int N = 17;

int main() {
  const std::string state_file_name = "state.json";
  auto ca = Automaton::loadFromFile(state_file_name, nullptr);
  auto protocol = IrradiationProtocol::loadFromFile("ins/3/2")[0];
  ca.setIrradiationProtocol(protocol);

  std::random_device rd{};
  for (int i = 0; i < N; ++i) {
    auto test_ca = ca;
    StdRandomEngine sre(rd());
    test_ca.setRandomEngine(&sre);
    for (int ss = 0; ss < 20; ++ss) {
      test_ca.runNSteps(1 * 12 * 600);
      auto &state = test_ca.getState();
      std::cout << countLiving(state) << " " << std::flush;
    }
    std::cout << std::endl;
    std::cerr << "Done: " << i + 1 << " " << std::endl;
  }
  return 0;
}