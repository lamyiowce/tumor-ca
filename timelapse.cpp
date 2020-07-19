#include <iostream>
#include <fstream>
#include <experimental/filesystem>
#include <Automaton.h>
#include <chrono>

namespace fs = std::experimental::filesystem;

const int N = 100;

int countLiving(const State &state) {
  int result = 0;
  for (ul r = 0; r < state.gridSize; ++r) {
    for (ul c = 0; c < state.gridSize; ++c) {
      if (state.W(r, c)) ++result;
    }
  }
  return result;
}

int countPhase(const State &state, State::CellCycle phase) {
  int result = 0;
  for (ul r = 0; r < state.gridSize; ++r) {
    for (ul c = 0; c < state.gridSize; ++c) {
      if (state.W(r, c) && state.cellCycle(r, c) == phase) ++result;
    }
  }
  return result;
}

int main(int argc, char **argv) {
  if (argc < 3) {
    std::cerr << "Usage: " << argv[0]
              << " tumor_file output_file" << std::endl;
    return 1;
  }
  const std::string state_file_name = argv[1];
  auto ca = Automaton::loadFromFile(state_file_name, nullptr);
  auto output_file = std::string(argv[2]);
  auto protocol = IrradiationProtocol(
      std::vector<ul>{1, 10801, 21601, 32401, 43201},
      std::vector<double>{2, 2, 2, 2, 2}
      );
//
//  auto protocol = IrradiationProtocol(
//      std::vector<ul>{1,
//                      10201,
//                      20401,
//                      30601,
//                      40801,
//                      51001,
//                      61201,
//                      71401},
//      std::vector<double>{1.25, 1.25, 1.25, 1.25, 1.25, 1.25, 1.25, 1.25}
//  );

  ca.setIrradiationProtocol(protocol);
  std::ofstream result_file(output_file);
  if (!result_file.good()) {
    std::cerr << "Could not create file: " << output_file << std::endl;
    return 1;
  }

  std::random_device rd{};
  for (int i = 0; i < N; ++i) {
    auto test_ca = ca;
    StdRandomEngine sre(rd());
    test_ca.setRandomEngine(&sre);
    auto &state = test_ca.getState();
    result_file << countLiving(state) << " ";
    result_file << countPhase(state, State::CellCycle::G1) << " ";
    result_file << countPhase(state, State::CellCycle::S) << " ";
    result_file << countPhase(state, State::CellCycle::G2)
                   + countPhase(state, State::CellCycle::M) << " ";
    result_file << state.CHO(25, 25) * 1e15 << " ";
    result_file << state.OX(25, 25)  * 1e15 << " ";
    result_file << state.GI(25, 25) * 1e15 << std::endl;
    auto start = std::chrono::steady_clock::now();
    for (int t = 0; t < 10 * 24 * 60; ++t) {
      test_ca.runNSteps(10);
      result_file << countLiving(state) << " ";
      result_file << countPhase(state, State::CellCycle::G1) << " ";
      result_file << countPhase(state, State::CellCycle::S) << " ";
      result_file << countPhase(state, State::CellCycle::G2)
                     + countPhase(state, State::CellCycle::M) << " ";
      result_file << state.CHO(25, 25) * 1e15 << " ";
      result_file << state.OX(25, 25)  * 1e15 << " ";
      result_file << state.GI(25, 25) * 1e15 << std::endl;
    }
    auto end = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << std::endl;
    std::cout << "Done: " << i + 1 << " " << std::endl;
  }

  return 0;
}
