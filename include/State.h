#ifndef TUMOR_STATE_H
#define TUMOR_STATE_H

#include <vector>
#include <util.h>
#include <json.hpp>

template <class T>
using grid = std::vector<T>;

class State {
public:
    enum class CellState {
        DEAD,
        AEROBIC_PROLIFERATION,
        ANAREOBIC_PROLIFERATION,
        AEROBIC_QUIESCENCE,
        ANAREOBIC_QUIESCENCE
    };

	enum class CellCycle {
		G1,  // 0
		S,   // 1
		G2,  // 2
		M,   // 3
		D    // 4
	};

    explicit State(ul gridSize);
    explicit State(nlohmann::json json);
    State(const State&) = default;
    ul gridSize;

private:
    grid<uint8_t> _W; // 1 - cell alive
    grid<float> _CHO; // CHO level
    grid<float> _OX; // oxygen level
    grid<float> _GI; // GI level
    grid<float> _timeInRepair; // STATE.RepT, time-in-repair clock
    grid<float> _irradiation; // STATE.R, the level of damage
                               //present in the site
    grid<CellState> _cellState; // biological state of a cell.
                                // holds the same information as STATE.lMET
    grid<CellCycle> _cellCycle; // ; STATE.gMET
    grid<float> _proliferationTime; // proliferation timer, same as STATE.HRS
	grid<bool> _cycleChanged; // STATE.ch; whether the site's cell cycle has
                              // changed during the last setGlobalStates() call

public:
    const uint8_t &W(ul r, ul c) const;

    uint8_t &W(ul r, ul c);

    const float &CHO(ul r, ul c) const;

    float &CHO(ul r, ul c);

    const float &OX(ul r, ul c) const;

    float &OX(ul r, ul c);

    const float &GI(ul r, ul c) const;

    float &GI(ul r, ul c);

    const float &timeInRepair(ul r, ul c) const;

    float &timeInRepair(ul r, ul c);

    const float &irradiation(ul r, ul c) const;

    float &irradiation(ul r, ul c);

    const CellState &cellState(ul r, ul c) const;

    CellState &cellState(ul r, ul c);

    const CellCycle &cellCycle(ul r, ul c) const;

    CellCycle &cellCycle(ul r, ul c);

    const float &proliferationTime(ul r, ul c) const;

    float &proliferationTime(ul r, ul c);

	bool cycleChanged(ul x, ul u);

	void setCycleChanged(ul r, ul c, bool value);

    float radius(ul r, ul c);

};


#endif //TUMOR_STATE_H
