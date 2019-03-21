
#ifndef TUMOR_IRRADIATIONPROTOCOL_H
#define TUMOR_IRRADIATIONPROTOCOL_H

#include "util.h"
#include "json.hpp"

class IrradiationProtocol {
    std::vector<ul> times;
    std::vector<double_p> doses;

public:
    explicit IrradiationProtocol(const nlohmann::json &json);

    explicit IrradiationProtocol(std::vector<ul> &&steps, std::vector<double_p> &&doses);

    double_p getIrradiationDose(ul step) const;

    static std::vector<IrradiationProtocol> loadFromFile(const std::string& filepath);
    
    void saveToFile(const std::string& filepath);
    
    bool sameAs(const IrradiationProtocol& other);
};


#endif //TUMOR_IRRADIATIONPROTOCOL_H
