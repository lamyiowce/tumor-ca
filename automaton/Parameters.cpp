
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

Parameters::Parameters(const nlohmann::json &json)
    : Parameters::Parameters(
        json["PARAMS"]["sCHOex"].get<double>(),
        json["PARAMS"]["sOXex"].get<double>(),
        Parameters::IrradiationProtocol(json),
        json["PARAMS"]["tau"].get<double>(),
        json["PARAMS"]["tstep"].get<double>(),
        json["PARAMS"]["sGIcrit"].get<double>(),
        json["PARAMS"]["sGIdeath"].get<double>(),
        json["PARAMS"]["siGI_n"].get<int>(),
        Parameters::Metabolism(json),
        json["PARAMS"]["Rmax"].get<double>(),
        {
            Parameters::NormDistParams(json["PARAMS"]["mCyc_G1"], json["PARAMS"]["sCyc_G1"]),
            Parameters::NormDistParams(json["PARAMS"]["mCyc_S"], json["PARAMS"]["sCyc_S"]),
            Parameters::NormDistParams(json["PARAMS"]["mCyc_G2"], json["PARAMS"]["sCyc_G2"]),
            Parameters::NormDistParams(json["PARAMS"]["mCyc_M"], json["PARAMS"]["sCyc_M"]),
            Parameters::NormDistParams(json["PARAMS"]["mCyc_D"], json["PARAMS"]["sCyc_D"])
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

Parameters::IrradiationProtocol::IrradiationProtocol(const nlohmann::json &json) {
    bool numbers = false;
    try {
        json["PARAMS"]["irr_f_times"].get<std::vector<ul>>();
    } catch (const nlohmann::detail::type_error &_) {
        numbers = true;
    }
    if (numbers) {
        times = std::vector<ul>(1, json["PARAMS"]["irr_f_times"].get<ul>());
        doses = std::vector<double>(1, json["PARAMS"]["irr_f_doses"].get<double>());
    } else {
        times = json["PARAMS"]["irr_f_times"].get<std::vector<ul>>();
        doses = json["PARAMS"]["irr_f_doses"].get<std::vector<double>>();
    }
}

Parameters::IrradiationProtocol::IrradiationProtocol(const std::vector<std::pair<ul, double>> &t_d_pairs) {
    for (auto d: t_d_pairs) {
        times.push_back(d.first);
        doses.push_back(d.second);
    }
}

double Parameters::IrradiationProtocol::getIrradiationDose(ul step) const {
    auto find = std::lower_bound(times.begin(), times.end(), step);
    if (find == times.end()) return 0.0;
    return doses[find - times.begin()];
}
