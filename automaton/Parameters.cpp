
#include <Parameters.h>
#include <utility>

Parameters::Parameters(
            double_p sCHOex,
            double_p sOXex,
            double_p sGIex,
            IrradiationProtocol irradiationSteps,
            double_p tau,
            double_p stepTime,
            double_p GIcritical,
            double_p GIdeath,
            int siGI_n,
            const Metabolism &metabolism,
            double_p rmax,
            const BirthParams &birthParams,
            double_p sDCHO,
            double_p sDOX,
            double_p sDGI
        )
        : sCHOex(sCHOex),
          sOXex(sOXex),
          sGIex(sGIex),
          irradiationSteps(std::move(irradiationSteps)),
          tau(tau),
          stepTime(stepTime),
          GIcritical(GIcritical),
          GIdeath(GIdeath),
          siGI_n(siGI_n),
          metabolism(metabolism),
          rMax(rmax),
          birthParams(birthParams),
          sDCHO(sDCHO),
          sDOX(sDOX),
          sDGI(sDGI) {}

Parameters::Parameters(const nlohmann::json &json)
    : Parameters::Parameters(
        json["PARAMS"]["sCHOex"].get<double_p>(),
        json["PARAMS"]["sOXex"].get<double_p>(),
        json["PARAMS"]["sGIex"].get<double_p>(),
        IrradiationProtocol(json),
        json["PARAMS"]["tau"].get<double_p>(),
        json["PARAMS"]["tstep"].get<double_p>(),
        json["PARAMS"]["sGIcrit"].get<double_p>(),
        json["PARAMS"]["sGIdeath"].get<double_p>(),
        json["PARAMS"]["siGI_n"].get<int>(),
        Parameters::Metabolism(json),
        json["PARAMS"]["Rmax"].get<double_p>(),
        {
            Parameters::NormDistParams(json["PARAMS"]["mCyc_G1"], json["PARAMS"]["sCyc_G1"]),
            Parameters::NormDistParams(json["PARAMS"]["mCyc_S"], json["PARAMS"]["sCyc_S"]),
            Parameters::NormDistParams(json["PARAMS"]["mCyc_G2"], json["PARAMS"]["sCyc_G2"]),
            Parameters::NormDistParams(json["PARAMS"]["mCyc_M"], json["PARAMS"]["sCyc_M"]),
            Parameters::NormDistParams(json["PARAMS"]["mCyc_D"], json["PARAMS"]["sCyc_D"])
        },
        json["PARAMS"]["sDcho"].get<double_p>(),
        json["PARAMS"]["sDox"].get<double_p>(),
        json["PARAMS"]["sDgi"].get<double_p>()
    ) {}

void Parameters::setIrradiationProtocol(std::vector<ul> _steps, std::vector<double_p> _times) {
    IrradiationProtocol newProtocol(std::move(_steps), std::move(_times));
    irradiationSteps = std::move(newProtocol);
}

Parameters::NutrientsParameters::NutrientsParameters(double_p CHO, double_p OX, double_p GI) : CHO(CHO), OX(OX), GI(GI) {}

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
