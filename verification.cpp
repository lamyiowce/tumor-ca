#include <iostream>
#include <fstream>
#include <experimental/filesystem>
#include <Automaton.h>
#include <chrono>

namespace fs = std::experimental::filesystem;

const int N = 1;

int countLiving(const State &state) {
  int result = 0;
  for (ul r = 0; r < state.gridSize; ++r) {
    for (ul c = 0; c < state.gridSize; ++c) {
      if (state.W(r, c)) ++result;
    }
  }
  return result;
}

int main(int argc, char **argv) {
  if (argc < 5) {
    std::cerr << "Usage: " << argv[0]
              << " output_dir input_dir set_id protocol_id" << std::endl;
    return 1;
  }
  const std::string state_file_name = "state.json";
  auto ca = Automaton::loadFromFile(state_file_name, nullptr);
  auto output_dir = fs::path(argv[1]);
  auto input_dir = fs::path(argv[2]);
  std::string set_id = argv[3];
  std::string protocol_id = argv[4];
  auto protocol_file_name = input_dir / set_id / protocol_id;
  auto protocols = IrradiationProtocol::loadFromFile(protocol_file_name);
  if (protocols.empty()) {
    std::cerr << "Invalid protocol file: " << protocol_file_name << std::endl;
    return 1;
  }
  ca.setIrradiationProtocol(protocols[0]);
  auto out_file_name = output_dir / set_id / protocol_id;
  std::ofstream result_file(out_file_name);
  if (!result_file.good()) {
    std::cerr << "Could not create file: " << out_file_name << std::endl;
    return 1;
  }

  std::random_device rd{};
  for (int i = 0; i < N; ++i) {
    auto test_ca = ca;
    StdRandomEngine sre(rd());
    test_ca.setRandomEngine(&sre);
    auto start = std::chrono::steady_clock::now();
    test_ca.runNSteps(10 * 24 * 600);
    auto end = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << std::endl;
    auto &state = test_ca.getState();
    result_file << countLiving(state) << ", " << std::flush;
    std::cout << "Done: " << i + 1 << " " << std::endl;
  }

  return 0;
}