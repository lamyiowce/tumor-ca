
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
            double rmax,
            const BirthParams &birthParams
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
          rMax(rmax),
          birthParams(birthParams) {}

Parameters::NutrientsParameters::NutrientsParameters(double CHO, double OX, double GI) : CHO(CHO), OX(OX), GI(GI) {}

Parameters::Metabolism::Metabolism(const Parameters::NutrientsParameters &anaerobicProliferation,
                                   const Parameters::NutrientsParameters &aerobicProliferation,
                                   const Parameters::NutrientsParameters &anaerobicQuiescence,
                                   const Parameters::NutrientsParameters &aerobicQuiescence)
                                   : anaerobicProliferation(anaerobicProliferation),
                                   aerobicProliferation(aerobicProliferation),
                                   anaerobicQuiescence(anaerobicQuiescence),
                                   aerobicQuiescence(aerobicQuiescence) {}

Parameters::NormDistParams::NormDistParams(float mean, float stddev) : mean(mean), stddev(stddev) {}

Parameters::BirthParams::BirthParams(const Parameters::NormDistParams &G1time,
                                     const Parameters::NormDistParams &Stime,
                                     const Parameters::NormDistParams &G2time,
                                     const Parameters::NormDistParams &Mtime,
                                     const Parameters::NormDistParams &Dtime)
                                     : G1time(G1time),
                                     Stime(Stime),
                                     G2time(G2time),
                                     Mtime(Mtime),
                                     Dtime(Dtime) {}
