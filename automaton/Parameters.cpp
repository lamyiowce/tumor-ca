
#include <Parameters.h>
#include <utility>

Parameters::Parameters(
            double sCHOex,
            double sOXex,
            IrradiationProtocol irradiationSteps,
            double tau,
            double stepTime, // tStep
            const Metabolism &metabolism,
			double GIcritical,
			double GIdeath,
			int siGI_n
        )
        : sCHOex(sCHOex),
          sOXex(sOXex),
          irradiationSteps(std::move(irradiationSteps)),
          tau(tau),
          stepTime(stepTime),
          metabolism(metabolism),
		  GIcritical(GIcritical),
		  GIdeath(GIdeath),
		  siGI_n(siGI_n) {}

