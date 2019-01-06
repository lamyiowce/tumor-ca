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
        const double CHO;
        const double OX;
        const double GI;

        NutrientsParameters(double CHO, double OX, double GI);
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
        std::vector<double> doses;

    public:
        explicit IrradiationProtocol(const nlohmann::json &json);

        explicit IrradiationProtocol(const std::vector<std::pair<ul, double>> &t_d_pairs);

        double getIrradiationDose(ul step) const;

    };

    Parameters(double sCHOex,
            double sOXex,
            double sGIex,
            IrradiationProtocol irradiationPerStep,
            double tau,
            double stepTime,
            double GIcritical,
            double GIdeath,
            int siGI_n,
            const Metabolism &metabolism,
            double rMax,
            const BirthParams &birthParams,
            double sDCHO,
            double sDOX,
            double sDGI);

    explicit Parameters(const nlohmann::json &json);
    Parameters(const Parameters&) = default;

    const double sCHOex; // no idea yet what that is, used in replenishSubstrates, CHO
    const double sOXex; // same as above, oxygen
    const double sGIex;
    const IrradiationProtocol irradiationSteps; /// vector of pairs (step number, irradiation dose)
    const double tau;
    const double stepTime; // step time in seconds

	// Critical pH values -- as concentration of hydrogen ions ("GI" == "growth inhibitor")
	const double GIcritical; // concentration above this value causes switching to quiescent metabolism; sGIcrit
	const double GIdeath; // concentration above this value causes death; sGIdeath

	const int siGI_n; // TODO necrotic material produced? Why is it set to zero in tumor-lib?
    const Metabolism metabolism; /// nutrients metabolism parameters
    const double rMax;
    const BirthParams birthParams;

    const double sDCHO;
    const double sDOX;
    const double sDGI;

};


#endif //TUMOR_PARAMETERS_H
