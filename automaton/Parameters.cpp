
#include "Parameters.h"
#include <utility>

Parameters::Parameters(const double sCHOex, const double sOXex, IrradiationProtocol irradiationSteps, const double tau)
        : sCHOex(sCHOex),
          sOXex(sOXex),
          irradiationSteps(std::move(irradiationSteps)),
          tau(tau) {}

