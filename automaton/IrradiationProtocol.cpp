
#include <fstream>
#include <iostream>
#include <sstream>
#include "IrradiationProtocol.h"


IrradiationProtocol::IrradiationProtocol(const nlohmann::json &json) {
    bool numbers = false;
    try {
        json["PARAMS"]["irr_f_times"].get<std::vector<ul>>();
    } catch (const nlohmann::detail::type_error &_) {
        numbers = true;
    }
    if (numbers) {
        times = std::vector<ul>(1, json["PARAMS"]["irr_f_times"].get<ul>());
        doses = std::vector<double_p>(1, json["PARAMS"]["irr_f_doses"].get<double_p>());
    } else {
        times = json["PARAMS"]["irr_f_times"].get<std::vector<ul>>();
        doses = json["PARAMS"]["irr_f_doses"].get<std::vector<double_p>>();
    }
}

IrradiationProtocol::IrradiationProtocol(std::vector<ul> &&_times, std::vector<double_p> &&_doses) :
        times(_times), doses(_doses) {
    assert(_times.size() == _doses.size());
}

double_p IrradiationProtocol::getIrradiationDose(ul step) const {
    auto find = std::lower_bound(times.begin(), times.end(), step);
    if (find == times.end()) return 0.0;
    return doses[find - times.begin()];
}

std::vector<IrradiationProtocol> IrradiationProtocol::loadFromFile(const std::string &filepath) {
    std::ifstream protocolFile(filepath);
    std::vector<IrradiationProtocol> protocols;
    std::string line;
    ul step;
    double_p dose;

    while (std::getline(protocolFile, line))
    {
        std::vector<ul> stepVector;
        std::vector<double_p> doseVector;

        std::istringstream issSteps(line);
        while(issSteps >> step) {
            stepVector.push_back(step);
        }
        if (std::getline(protocolFile, line)) {
            std::istringstream issDoses(line);
            while(issDoses >> dose) {
                doseVector.push_back(dose);
            }
            protocols.emplace_back(std::move(stepVector), std::move(doseVector));
        } else {
            std::cerr << "WARNING: Input malformed." << std::endl;
        }
    }

    return protocols;
}

bool IrradiationProtocol::sameAs(const IrradiationProtocol &other) {
    if (doses.size() != other.doses.size() ||
        times.size() != other.times.size())
        return false;
    
    for (ul i = 0; i < doses.size(); i++) {
        if (doses[i] != other.doses[i]) 
            return false;
        if (times[i] != other.times[i])
            return false;
    }
    return true;
}

void IrradiationProtocol::saveToFile(const std::string &filepath) {
    std::ofstream ofs(filepath);
    ofs << times;
    ofs << doses;
}
