
#include <Parameters.h>
#include <utility>

Parameters::Parameters(
            double sCHOex,
            double sOXex,
            IrradiationProtocol irradiationSteps,
            double tau,
            double stepTime,
            const Metabolism &metabolism,
            double rMax
        )
        : sCHOex(sCHOex),
          sOXex(sOXex),
          irradiationSteps(std::move(irradiationSteps)),
          tau(tau),
          stepTime(stepTime),
          metabolism(metabolism),
          rMax(rMax) {}

Parameters::NutrientsParameters::NutrientsParameters(double CHO, double OX, double GI) : CHO(CHO), OX(OX), GI(GI) {}

Parameters::Metabolism::Metabolism(const Parameters::NutrientsParameters &anaerobicProliferation,
                                   const Parameters::NutrientsParameters &aerobicProliferation,
                                   const Parameters::NutrientsParameters &anaerobicQuiescence,
                                   const Parameters::NutrientsParameters &aerobicQuiescence)
                                   : anaerobicProliferation(anaerobicProliferation),
                                   aerobicProliferation(aerobicProliferation),
                                   anaerobicQuiescence(anaerobicQuiescence),
                                   aerobicQuiescence(aerobicQuiescence) {}
