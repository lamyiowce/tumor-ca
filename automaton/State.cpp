
#include <State.h>
#include <cmath>

State::State(ul _gridSize) : gridSize(_gridSize),
                             _W(gridSize * gridSize),
                             _CHO(gridSize * gridSize),
                             _OX(gridSize * gridSize),
                             _GI(gridSize * gridSize),
                             _CH(gridSize * gridSize),
                             _timeInRepair(gridSize * gridSize),
                             _irradiation(gridSize * gridSize),
                             _cellState(gridSize * gridSize),
                             _cellCycle(gridSize * gridSize),
                             _proliferationTime(gridSize * gridSize),
							 _cycleChanged(gridSize * gridSize) {}

bool State::getW(ul x, ul y) const {
    return _W[y * gridSize + x];
}

void State::setW(ul x, ul y, bool value) {
    _W[y * gridSize + x] = value;
}

const double &State::CHO(ul x, ul y) const {
    return _CHO[y * gridSize + x];
}

double &State::CHO(ul x, ul y) {
    return _CHO[y * gridSize + x];
}

const double &State::OX(ul x, ul y) const {
    return _OX[y * gridSize + x];
}

double &State::OX(ul x, ul y) {
    return _OX[y * gridSize + x];
}

const double &State::GI(ul x, ul y) const {
    return _GI[y * gridSize + x];
}

double &State::GI(ul x, ul y) {
    return _GI[y * gridSize + x];
}

const double &State::CH(ul x, ul y) const {
    return _CH[y * gridSize + x];
}

double &State::CH(ul x, ul y) {
    return _CH[y * gridSize + x];
}

const double &State::timeInRepair(ul x, ul y) const {
    return _timeInRepair[y * gridSize + x];
}

double &State::timeInRepair(ul x, ul y) {
    return _timeInRepair[y * gridSize + x];
}

const double &State::irradiation(ul x, ul y) const {
    return _irradiation[y * gridSize + x];
}

double &State::irradiation(ul x, ul y) {
    return _irradiation[y * gridSize + x];
}

const State::CellState &State::cellState(ul x, ul y) const {
    return _cellState[y * gridSize + x];
}

State::CellState &State::cellState(ul x, ul y) {
    return _cellState[y * gridSize + x];
}

const State::CellCycle &State::cellCycle(ul x, ul y) const {
    return _cellCycle[y * gridSize + x];
}

State::CellCycle &State::cellCycle(ul x, ul y) {
    return _cellCycle[y * gridSize + x];
}

const double &State::proliferationTime(ul x, ul y) const {
    return _proliferationTime[y * gridSize + x];
}

double &State::proliferationTime(ul x, ul y) {
    return _proliferationTime[y * gridSize + x];
}

bool State::cycleChanged(ul x, ul y) {
	return _cycleChanged[y * gridSize + x];
}

void State::setCycleChanged(ul x, ul y, bool value) {
	_cycleChanged[y * gridSize + x] = value;
}

double State::radius(ul x, ul y) {
    return sqrt((x - gridSize / 2.) * (x - gridSize / 2.) + (y - gridSize / 2.) * (y - gridSize / 2.));
}
