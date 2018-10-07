
#include "Parameters.h"
#include <utility>

Parameters::Parameters(const double sCHOex, const double sOXex, std::vector<double> irradiationPerStep, const double tau)
        : sCHOex(sCHOex),
          sOXex(sOXex),
          irradiationPerStep(std::move(irradiationPerStep)),
          tau(tau) {}

