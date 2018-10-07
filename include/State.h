
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

    grid<bool> _W; // 1 - cell alive
    grid<double> _CHO; // CHO level
    grid<double> _OX; // oxygen level
    grid<double> _timeInRepair; // STATE.RepT, time-in-repair clock
    grid<double> _irradiation; // STATE.R, the level of damage present in the site
public:
    bool getW(ul x, ul y) const;

    void setW(ul x, ul y, bool value);

    const double &CHO(ul x, ul y) const;

    double &CHO(ul x, ul y);

    const double &OX(ul x, ul y) const;

    double &OX(ul x, ul y);

    const double &timeInRepair(ul x, ul y) const;

    double &timeInRepair(ul x, ul y);

    const double &irradiation(ul x, ul y) const;

    double &irradiation(ul x, ul y);

};


#endif //TUMOR_STATE_H
