
#ifndef TUMOR_STATE_H
#define TUMOR_STATE_H

#include <vector>
#include <util.h>

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
    State(State&) = default;
    ul gridSize;

private:
    grid<bool> _W; // 1 - cell alive
    grid<double> _CHO; // CHO level
    grid<double> _OX; // oxygen level
    grid<double> _GI; // GI level
    grid<double> _CH; // STATE.ch - recorded changes from the 
                      // SetGlobalStates phase to be repaired in RepairCells
    grid<double> _timeInRepair; // STATE.RepT, time-in-repair clock
    grid<double> _irradiation; // STATE.R, the level of damage 
                               //present in the site
    grid<CellState> _cellState; // biological state of a cell. 
                                // holds the same information as STATE.lMET
    grid<CellCycle> _cellCycle; // ; STATE.gMET
    grid<double> _proliferationTime; // proliferation timer, same as STATE.HRS
	grid<bool> _cycleChanged; // whether the site's cell cycle has changed 
                              // during the last setGlobalStates() call

public:
    bool getW(ul x, ul y) const;

    void setW(ul x, ul y, bool value);

    const double &CHO(ul x, ul y) const;

    double &CHO(ul x, ul y);

    const double &OX(ul x, ul y) const;

    double &OX(ul x, ul y);

    const double &GI(ul x, ul y) const;

    double &GI(ul x, ul y);

    const double &CH(ul x, ul y) const;

    double &CH(ul x, ul y);

    const double &timeInRepair(ul x, ul y) const;

    double &timeInRepair(ul x, ul y);

    const double &irradiation(ul x, ul y) const;

    double &irradiation(ul x, ul y);

    const CellState &cellState(ul x, ul y) const;

    CellState &cellState(ul x, ul y);

    const CellCycle &cellCycle(ul x, ul y) const;

    CellCycle &cellCycle(ul x, ul y);

    const double &proliferationTime(ul x, ul y) const;

    double &proliferationTime(ul x, ul y);

	bool cycleChanged(ul x, ul u);

	void setCycleChanged(ul x, ul y, bool value);
};


#endif //TUMOR_STATE_H
