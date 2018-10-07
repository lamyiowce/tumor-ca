
#include <State.h>

#include "State.h"

State::State(ul _gridSize) : gridSize(_gridSize),
                             W(gridSize * gridSize),
                             CHO(gridSize * gridSize),
                             OX(gridSize * gridSize) {}

bool State::getW(ul x, ul y) const {
    return W[y * gridSize + x];
}

void State::setW(ul x, ul y, bool value) {
    W[y * gridSize + x] = value;
}

const double &State::getCHO(ul x, ul y) const {
    return CHO[y * gridSize + x];
}

double &State::getCHO(ul x, ul y) {
    return CHO[y * gridSize + x];
}

const double &State::getOX(ul x, ul y) const {
    return OX[y * gridSize + x];
}

double &State::getOX(ul x, ul y) {
    return OX[y * gridSize + x];
}

const double &State::getTimeInRepair(ul x, ul y) const {
    return timeInRepair[y * gridSize + x];
}

double &State::getTimeInRepair(ul x, ul y) {
    return timeInRepair[y * gridSize + x];
}

const double &State::getIrradiation(ul x, ul y) const {
    return irradiation[y * gridSize + x];
}

double &State::getIrradiation(ul x, ul y) {
    return irradiation[y * gridSize + x];
}
