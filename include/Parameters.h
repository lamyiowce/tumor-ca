#ifndef TUMOR_PARAMETERS_H
#define TUMOR_PARAMETERS_H
#include <vector>
#include <util.h>
#include <json.hpp>

using IrradiationProtocol = std::vector<std::pair<ul, double>>;

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
        NormDistParams G1time;
        NormDistParams Stime;
        NormDistParams G2time;
        NormDistParams Mtime;
        NormDistParams Dtime;

        BirthParams(const NormDistParams &G1time, const NormDistParams &Stime, const NormDistParams &G2time,
                    const NormDistParams &Mtime, const NormDistParams &Dtime);
    };

    Parameters(double sCHOex,
            double sOXex,
            IrradiationProtocol irradiationPerStep,
            double tau,
            double stepTime,
            double GIcritical,
            double GIdeath,
            int siGI_n,
            const Metabolism &metabolism,
            double rMax,
            const BirthParams &birthParams);

    explicit Parameters(nlohmann::json json);
    Parameters(Parameters&) = default;

    const double sCHOex; // no idea yet what that is, used in replenishSubstrates, CHO
    const double sOXex; // same as above, oxygen
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

};


#endif //TUMOR_PARAMETERS_H
