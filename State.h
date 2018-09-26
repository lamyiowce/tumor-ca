
#ifndef TUMOR_STATE_H
#define TUMOR_STATE_H

#include <vector>
#include "util.h"

template <class T>
using grid = std::vector<std::vector<T>>;
//template <size_t N> using Vector = Matrix<N, 1>;
//template<class Option, class T> using Vector = typename Option::template Vector<T>;

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
};


#endif //TUMOR_STATE_H
