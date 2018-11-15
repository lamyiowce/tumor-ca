
#include <Parameters.h>
#include <utility>

Parameters::Parameters(
            double sCHOex,
            double sOXex,
            IrradiationProtocol irradiationSteps,
            double tau,
            double stepTime,
            double GIcritical,
            double GIdeath,
            int siGI_n,
            const Metabolism &metabolism,
            double rmax
        )
        : sCHOex(sCHOex),
          sOXex(sOXex),
          irradiationSteps(std::move(irradiationSteps)),
          tau(tau),
          stepTime(stepTime),
          GIcritical(GIcritical),
          GIdeath(GIdeath),
          siGI_n(siGI_n),
          metabolism(metabolism),
          rMax(rmax) {}

Parameters::NutrientsParameters::NutrientsParameters(double CHO, double OX, double GI) : CHO(CHO), OX(OX), GI(GI) {}

Parameters::Metabolism::Metabolism(const Parameters::NutrientsParameters &anaerobicProliferation,
                                   const Parameters::NutrientsParameters &aerobicProliferation,
                                   const Parameters::NutrientsParameters &anaerobicQuiescence,
                                   const Parameters::NutrientsParameters &aerobicQuiescence)
                                   : anaerobicProliferation(anaerobicProliferation),
                                   aerobicProliferation(aerobicProliferation),
                                   anaerobicQuiescence(anaerobicQuiescence),
                                   aerobicQuiescence(aerobicQuiescence) {}
