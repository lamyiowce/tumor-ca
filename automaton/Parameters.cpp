
#include "Parameters.h"
#include <utility>

Parameters::Parameters(const double sCHOex, const double sOXex, IrradiationProtocol irradiationPerStep, const double tau)
        : sCHOex(sCHOex),
          sOXex(sOXex),
          irradiationSteps(std::move(irradiationPerStep)),
          tau(tau) {}

