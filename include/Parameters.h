#ifndef TUMOR_PARAMETERS_H
#define TUMOR_PARAMETERS_H
#include <vector>
#include <util.h>
#include <json.hpp>

class Parameters {
public:
    /**
     * Amounts of nutrients metabolised.
     */
    struct NutrientsParameters {
        const double_p CHO;
        const double_p OX;
        const double_p GI;

        NutrientsParameters(double_p CHO, double_p OX, double_p GI);
    };

    /**
     * Metabolism information for all biological states.
     */
    struct Metabolism {
        NutrientsParameters anaerobicProliferation; /// sCHOp_an, sOXp_an, sdGIp_an
        NutrientsParameters aerobicProliferation; /// sCHOp, sOXp, sdGIp
        NutrientsParameters anaerobicQuiescence; /// sCHOq_an, sOXq_an, sdGIq_an
        NutrientsParameters aerobicQuiescence; /// sCHOq, sOXq, sdGIq

        Metabolism(const NutrientsParameters &anaerobicProliferation, const NutrientsParameters &aerobicProliferation,
                   const NutrientsParameters &anaerobicQuiescence, const NutrientsParameters &aerobicQuiescence);
        explicit Metabolism(nlohmann::json json);
    };

    struct NormDistParams {
        float mean;
        float stddev;

        NormDistParams(float mean, float stddev);
    };

    struct BirthParams {
        NormDistParams G1time; // mCyc_G1, sCyc_G1
        NormDistParams Stime; // mCyc_S, sCyc_S
        NormDistParams G2time; // mCyc_G2, sCyc_G2
        NormDistParams Mtime; // mCyc_M, sCyc_M
        NormDistParams Dtime; // mCyc_D, sCyc_D

        BirthParams(const NormDistParams &G1time, const NormDistParams &Stime, const NormDistParams &G2time,
                    const NormDistParams &Mtime, const NormDistParams &Dtime);
    };

    class IrradiationProtocol {
        std::vector<ul> times;
        std::vector<double_p> doses;

    public:
        explicit IrradiationProtocol(const nlohmann::json &json);

        explicit IrradiationProtocol(const std::vector<std::pair<ul, double_p>> &t_d_pairs);

        double_p getIrradiationDose(ul step) const;

    };

    Parameters(double_p sCHOex,
            double_p sOXex,
            double_p sGIex,
            IrradiationProtocol irradiationPerStep,
            double_p tau,
            double_p stepTime,
            double_p GIcritical,
            double_p GIdeath,
            int siGI_n,
            const Metabolism &metabolism,
            double_p rMax,
            const BirthParams &birthParams,
            double_p sDCHO,
            double_p sDOX,
            double_p sDGI);

    explicit Parameters(const nlohmann::json &json);
    Parameters(const Parameters&) = default;

    const double_p sCHOex; // no idea yet what that is, used in replenishSubstrates, CHO
    const double_p sOXex; // same as above, oxygen
    const double_p sGIex;
    const IrradiationProtocol irradiationSteps; /// vector of pairs (step number, irradiation dose)
    const double_p tau;
    const double_p stepTime; // step time in seconds

	// Critical pH values -- as concentration of hydrogen ions ("GI" == "growth inhibitor")
	const double_p GIcritical; // concentration above this value causes switching to quiescent metabolism; sGIcrit
	const double_p GIdeath; // concentration above this value causes death; sGIdeath

	const int siGI_n; // TODO necrotic material produced? Why is it set to zero in tumor-lib?
    const Metabolism metabolism; /// nutrients metabolism parameters
    const double_p rMax;
    const BirthParams birthParams;

    const double_p sDCHO;
    const double_p sDOX;
    const double_p sDGI;

};


#endif //TUMOR_PARAMETERS_H
