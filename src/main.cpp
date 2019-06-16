#include <iostream>
#include <fstream>
#include <Automaton.h>
#include <RandomEngine.h>
#include <sstream>
#include <atomic>
#include <sys/stat.h>
#include <iomanip>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

int main(int argc, char* argv[]) {
    if (argc != 6) {
        std::cerr << "Usage: " << argv[0] << " <automaton file> <protocol file> <results dir> <experiment id> <nsteps>" << std::endl;
        return 0;
    }

    const fs::path automatonPath = fs::path(argv[1]);
    const fs::path protocolPath = fs::path(argv[2]);
    const fs::path resultsDir = fs::path(argv[3]);
    const std::string experimentId = argv[4];
    int nSteps;
    try {
        nSteps = std::stoi(argv[5]);
    } catch (const std::invalid_argument& e) {
        std::cerr << "Error in parsing nSteps." << std::endl;
        return 1;
    }

    auto ca = Automaton::loadFromFile(automatonPath, nullptr);
    auto protocols = IrradiationProtocol::loadFromFile(protocolPath);
    if (protocols.empty()) {
        std::cerr << "Error loading protocols or protocol file is empty." << std::endl;
        return 1;
    }

    const fs::path experimentDir = resultsDir / experimentId;
    const fs::path protocolDir = experimentDir / "protocols";
    const fs::path outDir = experimentDir / "out";

    try {
        fs::create_directories(resultsDir);
        if (fs::exists(experimentDir))
            fs::remove_all(experimentDir);
        else
            fs::create_directories(experimentDir);
        fs::create_directories(protocolDir);
        fs::create_directories(outDir);
    } catch (const fs::filesystem_error& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
    }

    std::cout << std::endl;
    std::cout << "====== CELLULAR AUTOMATON ======" << std::endl;
    std::cout << "Number of steps:     " << nSteps << std::endl;
    std::cout << "Results directory:   " << resultsDir << std::endl;
    std::cout << "Experiment id:       " << experimentId << std::endl;
    std::cout << "Starting CA:         " << automatonPath << std::endl;
    std::cout << "Number of protocols: " << protocols.size() << std::endl;
    std::cout << "Protocols file:      " << protocolPath << std::endl;
    std::cout << "================================" << std::endl;
    std::cout << std::endl;

    int digits = 0;
    auto x = protocols.size();
    do digits++; while (x/=10);

    int done = 0;
    std::random_device rd;
    std::cout << "PROGRESS: " << std::setw(digits) << done << "/" << protocols.size() << std::flush;
    #pragma omp parallel for schedule(guided)
    for (ul i = 0; i < protocols.size(); i++){
        auto outFilePath = outDir / (experimentId + "_" + std::to_string(i) + ".csv");
        auto protocolFilePath = protocolDir / (experimentId + "_" + std::to_string(i) + ".csv");
        protocols[i].saveToFile(protocolFilePath);
        auto myCa = ca;
        ul seed;
        #pragma omp critical
            seed = rd();
        StdRandomEngine myRandEngine(seed);
        myCa.setRandomEngine(&myRandEngine);
        myCa.setIrradiationProtocol(std::move(protocols[i]));
        myCa.runNSteps(nSteps);
        myCa.saveStateToFile(outFilePath);

        #pragma omp critical
            std::cout << '\r' << "PROGRESS: " << std::setw(digits) << ++done << "/" << protocols.size() << std::flush;
    }
    return 0;
}
