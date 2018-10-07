
#ifndef TUMOR_PARAMETERS_H
#define TUMOR_PARAMETERS_H
#include <vector>
#include "util.h"

using IrradiationProtocol = std::vector<std::pair<ul, double>>;

class Parameters {
public:
    Parameters(double sCHOex, double sOXex, IrradiationProtocol irradiationPerStep, double tau);

    Parameters(Parameters&) = default;

    const double sCHOex; // no idea yet what that is, used in replenishSubstrates, CHO
    const double sOXex; // same as above, oxygen
    const IrradiationProtocol irradiationSteps; // vector of pairs (step number, irradiation dose)
    const double tau;
};


#endif //TUMOR_PARAMETERS_H
