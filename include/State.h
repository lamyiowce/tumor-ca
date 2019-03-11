#ifndef TUMOR_STATE_H
#define TUMOR_STATE_H

#include <vector>
#include <util.h>
#include <json.hpp>
#include <fstream>

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
    grid<single_p> _CHO; // CHO level
    grid<single_p> _OX; // oxygen level
    grid<single_p> _GI; // GI level
    grid<single_p> _timeInRepair; // STATE.RepT, time-in-repair clock
    grid<single_p> _irradiation; // STATE.R, the level of damage
                               //present in the site
    grid<CellState> _cellState; // biological state of a cell.
                                // holds the same information as STATE.lMET
    grid<CellCycle> _cellCycle; // ; STATE.gMET
    grid<single_p> _proliferationTime; // proliferation timer, same as STATE.HRS
	grid<bool> _cycleChanged; // STATE.ch; whether the site's cell cycle has
                              // changed during the last setGlobalStates() call

public:
    const uint8_t &W(ul r, ul c) const;

    uint8_t &W(ul r, ul c);

    const single_p &CHO(ul r, ul c) const;

    single_p &CHO(ul r, ul c);

    const single_p &OX(ul r, ul c) const;

    single_p &OX(ul r, ul c);

    const single_p &GI(ul r, ul c) const;

    single_p &GI(ul r, ul c);

    const single_p &timeInRepair(ul r, ul c) const;

    single_p &timeInRepair(ul r, ul c);

    const single_p &irradiation(ul r, ul c) const;

    single_p &irradiation(ul r, ul c);

    const CellState &cellState(ul r, ul c) const;

    CellState &cellState(ul r, ul c);

    const CellCycle &cellCycle(ul r, ul c) const;

    CellCycle &cellCycle(ul r, ul c);

    const single_p &proliferationTime(ul r, ul c) const;

    single_p &proliferationTime(ul r, ul c);

	bool cycleChanged(ul x, ul u);

	void setCycleChanged(ul r, ul c, bool value);

    single_p radius(ul r, ul c);

	friend std::ofstream& operator<<(std::ofstream& stream, const State& state);
};


#endif //TUMOR_STATE_H
