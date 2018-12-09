
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

Parameters::Parameters(nlohmann::json json)
    : Parameters::Parameters(
        json["PARAMS"]["sCHOex"],
        json["PARAMS"]["sOXex"],
        {{0, 1}, {2, 3.0}, {3, 0.3}, {4, 0.5}},  // TODO irradiationSteps
        json["PARAMS"]["tau"],
        json["PARAMS"]["tstep"],
        json["PARAMS"]["sGIcrit"],
        json["PARAMS"]["sGIdeath"],
        json["PARAMS"]["siGI_n"],
        Parameters::Metabolism(json),
        json["PARAMS"]["Rmax"],
        {
            // TODO? skad te wartosci sie biora?
            Parameters::NormDistParams(0.1, 0.1),
            Parameters::NormDistParams(0.1, 0.1),
            Parameters::NormDistParams(0.1, 0.1),
            Parameters::NormDistParams(0.1, 0.1),
            Parameters::NormDistParams(0.1, 0.1)
        }
    ) {}

Parameters::NutrientsParameters::NutrientsParameters(double CHO, double OX, double GI) : CHO(CHO), OX(OX), GI(GI) {}

Parameters::Metabolism::Metabolism(const Parameters::NutrientsParameters &anaerobicProliferation,
                                   const Parameters::NutrientsParameters &aerobicProliferation,
                                   const Parameters::NutrientsParameters &anaerobicQuiescence,
                                   const Parameters::NutrientsParameters &aerobicQuiescence)
                                   : anaerobicProliferation(anaerobicProliferation),
                                   aerobicProliferation(aerobicProliferation),
                                   anaerobicQuiescence(anaerobicQuiescence),
                                   aerobicQuiescence(aerobicQuiescence) {}

Parameters::Metabolism::Metabolism(nlohmann::json json)
    : Parameters::Metabolism::Metabolism(
        {json["PARAMS"]["sCHOp_an"], json["PARAMS"]["sOXp_an"], json["PARAMS"]["sdGIp_an"]},
        {json["PARAMS"]["sCHOp"], json["PARAMS"]["sOXp"], json["PARAMS"]["sdGIp"]},
        {json["PARAMS"]["sCHOq_an"], json["PARAMS"]["sOXq_an"], json["PARAMS"]["sdGIq_an"]},
        {json["PARAMS"]["sCHOq"], json["PARAMS"]["sOXq"], json["PARAMS"]["sdGIq"]}
    ) {}

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
