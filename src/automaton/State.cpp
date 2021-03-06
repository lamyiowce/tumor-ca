#include <State.h>
#include <cmath>

State::State(ul _gridSize) : gridSize(_gridSize),
                             _W(gridSize * gridSize),
                             _CHO(gridSize * gridSize),
                             _OX(gridSize * gridSize),
                             _GI(gridSize * gridSize),
                             _timeInRepair(gridSize * gridSize),
                             _irradiation(gridSize * gridSize),
                             _cellState(gridSize * gridSize),
                             _cellCycle(gridSize * gridSize),
                             _proliferationTime(gridSize * gridSize),
                             _cycleChanged(gridSize * gridSize) {}

static ul indexFromMatlab(ul matlabIndex, ul gridSize) {
    return ((matlabIndex - 1) % gridSize) * gridSize + (matlabIndex - 1) / gridSize;
}

State::State(nlohmann::json json)
    : State::State(static_cast<ul>(json["PARAMS"]["L"])) {
    for (ul r = 0; r < gridSize; ++r) {
        for (ul c = 0; c < gridSize; ++c) {
            this->W(r, c) = json["STATE"]["W"][r][c];
            this->CHO(r, c) = json["STATE"]["CHO"][r][c];
            this->OX(r, c) = json["STATE"]["OX"][r][c];
            this->GI(r, c) = json["STATE"]["GI"][r][c];
            this->proliferationTime(r, c) = json["STATE"]["HRS"][r][c];
            this->timeInRepair(r, c) = json["STATE"]["RepT"][r][c];
            this->irradiation(r, c) = json["STATE"]["R"][r][c];
            this->cellCycle(r, c)
                = static_cast<State::CellCycle >(json["STATE"]["gMET"][r][c]);
            this->cycleChanged(r, c) = json["STATE"]["ch"][r][c];
        }
    }

    for (ul matlab_idx : json["STATE"]["lMET"]["prolif"]) {
        this->_cellState[indexFromMatlab(matlab_idx, gridSize)] = State::CellState::AEROBIC_PROLIFERATION;
    }

    for (ul matlab_idx : json["STATE"]["lMET"]["prolif_an"]) {
        this->_cellState[indexFromMatlab(matlab_idx, gridSize)] = State::CellState::ANAREOBIC_PROLIFERATION;
    }

    for (ul matlab_idx : json["STATE"]["lMET"]["quiesc"]) {
        this->_cellState[indexFromMatlab(matlab_idx, gridSize)] = State::CellState::AEROBIC_QUIESCENCE;
    }

    for (ul matlab_idx : json["STATE"]["lMET"]["quiesc_an"]) {
        this->_cellState[indexFromMatlab(matlab_idx, gridSize)] = State::CellState::ANAREOBIC_QUIESCENCE;
    }

    for (ul matlab_idx : json["STATE"]["lMET"]["dead"]) {
        this->_cellState[indexFromMatlab(matlab_idx, gridSize)] = State::CellState::DEAD;
    }
}

const uint8_t &State::W(ul r, ul c) const {
    return _W[r * gridSize + c];
}

uint8_t &State::W(ul r, ul c) {
    return _W[r * gridSize + c];
}

const single_p &State::CHO(ul r, ul c) const {
    return _CHO[r * gridSize + c];
}

single_p &State::CHO(ul r, ul c) {
    return _CHO[r * gridSize + c];
}

const single_p &State::OX(ul r, ul c) const {
    return _OX[r * gridSize + c];
}

single_p &State::OX(ul r, ul c) {
    return _OX[r * gridSize + c];
}

const single_p &State::GI(ul r, ul c) const {
    return _GI[r * gridSize + c];
}

single_p &State::GI(ul r, ul c) {
    return _GI[r * gridSize + c];
}

const single_p &State::timeInRepair(ul r, ul c) const {
    return _timeInRepair[r * gridSize + c];
}

single_p &State::timeInRepair(ul r, ul c) {
    return _timeInRepair[r * gridSize + c];
}

const single_p &State::irradiation(ul r, ul c) const {
    return _irradiation[r * gridSize + c];
}

single_p &State::irradiation(ul r, ul c) {
    return _irradiation[r * gridSize + c];
}

const State::CellState &State::cellState(ul r, ul c) const {
    return _cellState[r * gridSize + c];
}

State::CellState &State::cellState(ul r, ul c) {
    return _cellState[r * gridSize + c];
}

const State::CellCycle &State::cellCycle(ul r, ul c) const {
    return _cellCycle[r * gridSize + c];
}

State::CellCycle &State::cellCycle(ul r, ul c) {
    return _cellCycle[r * gridSize + c];
}

const single_p &State::proliferationTime(ul r, ul c) const {
    return _proliferationTime[r * gridSize + c];
}

single_p &State::proliferationTime(ul r, ul c) {
    return _proliferationTime[r * gridSize + c];
}

const uint8_t &State::cycleChanged(ul r, ul c) const {
	return _cycleChanged[r * gridSize + c];
}

uint8_t &State::cycleChanged(ul r, ul c) {
	return _cycleChanged[r * gridSize + c];
}

single_p State::radius(ul r, ul c) {
    return sqrt((r - gridSize / 2.) * (r - gridSize / 2.) + (c - gridSize / 2.) * (c - gridSize / 2.));
}

std::ofstream& operator<<(std::ofstream& stream, const State& state)
{
    stream << state._W << state._CHO << state._OX << state._GI << state._timeInRepair << state._irradiation
           << state._cellState << state._cellCycle << state._proliferationTime << state._cycleChanged;
    return stream;
}