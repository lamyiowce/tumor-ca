
#ifndef TUMOR_STATE_H
#define TUMOR_STATE_H

#include <vector>
#include "util.h"

template <class T>
using grid = std::vector<T>;

class State {
public:
    explicit State(unsigned long gridSize);
    State(State&) = default;
    ul gridSize;

    grid<bool> W; // 1 - cell alive
    grid<double> CHO; // CHO level
    grid<double> OX; // oxygen level
    grid<double> timeInRepair; // STATE.RepT, time-in-repair clock
    grid<double> irradiation; // STATE.R, the level of damage present in the site
public:
    bool getW(ul x, ul y) const;

    void setW(ul x, ul y, bool value);

    const double &getCHO(ul x, ul y) const;

    double &getCHO(ul x, ul y);

    const double &getOX(ul x, ul y) const;

    double &getOX(ul x, ul y);

    const double &getTimeInRepair(ul x, ul y) const;

    double &getTimeInRepair(ul x, ul y);

    const double &getIrradiation(ul x, ul y) const;

    double &getIrradiation(ul x, ul y);

};


#endif //TUMOR_STATE_H
