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
        const float CHO;
        const float OX;
        const float GI;

        NutrientsParameters(float CHO, float OX, float GI);
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
        std::vector<float> doses;

    public:
        explicit IrradiationProtocol(const nlohmann::json &json);

        explicit IrradiationProtocol(const std::vector<std::pair<ul, float>> &t_d_pairs);

        float getIrradiationDose(ul step) const;

    };

    Parameters(float sCHOex,
            float sOXex,
            float sGIex,
            IrradiationProtocol irradiationPerStep,
            float tau,
            float stepTime,
            float GIcritical,
            float GIdeath,
            int siGI_n,
            const Metabolism &metabolism,
            float rMax,
            const BirthParams &birthParams,
            float sDCHO,
            float sDOX,
            float sDGI);

    explicit Parameters(const nlohmann::json &json);
    Parameters(const Parameters&) = default;

    const float sCHOex; // no idea yet what that is, used in replenishSubstrates, CHO
    const float sOXex; // same as above, oxygen
    const float sGIex;
    const IrradiationProtocol irradiationSteps; /// vector of pairs (step number, irradiation dose)
    const float tau;
    const float stepTime; // step time in seconds

	// Critical pH values -- as concentration of hydrogen ions ("GI" == "growth inhibitor")
	const float GIcritical; // concentration above this value causes switching to quiescent metabolism; sGIcrit
	const float GIdeath; // concentration above this value causes death; sGIdeath

	const int siGI_n; // TODO necrotic material produced? Why is it set to zero in tumor-lib?
    const Metabolism metabolism; /// nutrients metabolism parameters
    const float rMax;
    const BirthParams birthParams;

    const float sDCHO;
    const float sDOX;
    const float sDGI;

};


#endif //TUMOR_PARAMETERS_H
