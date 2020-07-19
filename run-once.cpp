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

void printInfo(const State &state) {
  std::cout << state.CHO(25, 25) * 1e15 << ", ";
  std::cout << state.OX(25, 25) * 1e15 << ", ";
  std::cout << state.GI(25, 25) * 1e15 << ", ";
  ul alive = 0;
  ul in_state[4] = {0, 0, 0, 0};
  ul in_cycle[5] = {0, 0, 0, 0, 0};
  for (ul r = 0; r < 51; ++r) {
    for (ul c = 0; c < 51; ++c) {
      if (state.W(r, c)) {
        alive++;
        if (static_cast<ul>(state.cellState(r, c)) >= 2) {
          in_state[static_cast<ul>(state.cellState(r, c)) - 2]++;
        }
        in_cycle[static_cast<ul>(state.cellCycle(r, c))]++;
      }
    }
  }
  std::cout << alive << ", ";
  for (ul i = 0; i < 4; ++i) std::cout << in_state[i] << ", ";
  for (ul i = 0; i < 5; ++i) std::cout << in_cycle[i] << ", ";
  std::cout << "\n";
}

const int N = 10;

int main() {
  const int64_t hour_steps = 600;
  // std::vector<ul> times(8, 8 * hour_steps + 1);
  // for (ul d = 0; d < 8; ++d) {
  //   if (d % 2 == 1) times[d] += 8 * hour_steps;
  //   times[d] += (d/2) * 24 * hour_steps;
  // }
  // auto protocol = IrradiationProtocol(std::move(times), std::vector<double>(8, 1.25));

  auto protocol = IrradiationProtocol::loadFromFile("protocol.txt")[0];

  std::random_device rd{};
  for (int t = 3; t < 4; ++t) {
    const std::string state_file_name = "tumor-lib/tumor-" + std::to_string(t) + ".json";
    auto ca = Automaton::loadFromFile(state_file_name, nullptr);
    ca.setIrradiationProtocol(protocol);
    for (int i = 0; i < N; ++i) {
      auto test_ca = ca;
      StdRandomEngine sre(rd());
      test_ca.setRandomEngine(&sre);
      auto &state = test_ca.getState();
      printInfo(state);
      for (int s = 0; s < 6 * 24 * 10; ++s) {
        test_ca.runNSteps(100);
        auto &state = test_ca.getState();
        printInfo(state);
      }
    }
  }
  return 0;
}
