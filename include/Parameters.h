
#ifndef TUMOR_PARAMETERS_H
#define TUMOR_PARAMETERS_H
#include <vector>
#include <util.h>

using IrradiationProtocol = std::vector<std::pair<ul, double>>;

class Parameters {
public:
    /**
     * Amounts of nutrients metabolised.
     */
    struct NutrientsParameters {
        double CHO;
        double OX;
        double GI;

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
    };

    Parameters(double sCHOex,
            double sOXex,
            IrradiationProtocol irradiationPerStep,
            double tau,
            double stepTime,
            const Metabolism &metabolism,
            double rMax);

    Parameters(Parameters&) = default;

    const double sCHOex; // no idea yet what that is, used in replenishSubstrates, CHO
    const double sOXex; // same as above, oxygen
    const IrradiationProtocol irradiationSteps; /// vector of pairs (step number, irradiation dose)
    const double tau;
    const double stepTime; // step time in seconds
    const Metabolism metabolism; /// nutrients metabolism parameters
    const double rMax;

};


#endif //TUMOR_PARAMETERS_H
