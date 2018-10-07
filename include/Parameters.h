
#ifndef TUMOR_PARAMETERS_H
#define TUMOR_PARAMETERS_H
#include <vector>

class Parameters {
public:
    Parameters(double sCHOex, double sOXex, std::vector<double> irradiationPerStep, double tau);

    Parameters(Parameters&) = default;

    const double sCHOex; // no idea yet what that is, used in replenishSubstrates, CHO
    const double sOXex; // same as above, oxygen
    const std::vector<double> irradiationPerStep; // irradiationPerStep[i] has the dose of irradiation in ith step; no irrad. if == 0
    const double tau;
};


#endif //TUMOR_PARAMETERS_H
