#include <iostream>
#include <fstream>
#include <Automaton.h>
#include <RandomEngine.h>
#include <sstream>
#include <atomic>
#include <sys/stat.h>
#include <iomanip>

int main(int argc, char* argv[]) {
    StdRandomEngine sre(10009);
    if (argc != 6) {
        std::cerr << "Usage: " << argv[0] << " <automaton file> <protocol file> <results dir> <experiment id> <nsteps>" << std::endl;
        return 0;
    }

    const std::string automatonPath = argv[1];
    const std::string protocolPath = argv[2];
    const std::string resultsDir = argv[3];
    const std::string experimentId = argv[4];
    int nSteps;
    try {
        nSteps = std::stoi(argv[5]);
    } catch (const std::invalid_argument& e) {
        std::cerr << "Error in parsing nSteps." << std::endl;
        return 1;
    }

    auto ca = Automaton::loadFromFile(automatonPath, &sre);
    auto protocols = IrradiationProtocol::loadFromFile(protocolPath);

    const std::string experimentDir = resultsDir + "/" + experimentId;
    const std::string protocolDir = experimentDir + "/protocols";
    const std::string outDir = experimentDir + "/out";

    mode_t mode = 0752;
    for (const std::string& dir : {resultsDir, experimentDir, protocolDir, outDir}) {
        auto status = mkdir(dir.data(), mode);
        if (status < 0 && errno != EEXIST) {
            std::cerr << strerror(errno) << ": Could not create directory: " << dir << std::endl;
            return 1;
        }
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
    std::cout << "PROGRESS: " << std::setw(digits) << done << "/" << protocols.size() << std::flush;
    #pragma omp parallel for schedule(guided)
    for (ul i = 0; i < protocols.size(); i++){
        auto myCa = ca;
        myCa.setIrradiationProtocol(std::move(protocols[i]));
        myCa.runNSteps(nSteps);
        auto outFilePath = outDir + "/" + std::to_string(i) + ".csv";
        auto protocolFilePath = protocolDir + "/" + std::to_string(i) + ".csv";
        std::ofstream caFile(outFilePath);
        std::ofstream protocolFile(protocolFilePath);
        caFile << "CA HERE" << std::endl;
        protocolFile << "PROTOCOL HERE" << std::endl;

        #pragma omp critical
            std::cout << '\r' << "PROGRESS: " << std::setw(digits) << ++done << "/" << protocols.size() << std::flush;

        // TODO: Actually saving the result.
    }
    return 0;
}
