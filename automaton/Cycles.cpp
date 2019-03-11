#include <Cycles.h>

Cycles::Cycles(unsigned long gridSize) :_gridSize(gridSize) {
	this->_G1time = std::vector<single_p>(_gridSize * _gridSize);
	this->_Stime = std::vector<single_p>(_gridSize * _gridSize);
	this->_G2time = std::vector<single_p>(_gridSize * _gridSize);
	this->_Mtime = std::vector<single_p>(_gridSize * _gridSize);
	this->_Dtime = std::vector<single_p>(_gridSize * _gridSize);
}

Cycles::Cycles(nlohmann::json json)
    : Cycles::Cycles(static_cast<unsigned long>(json["PARAMS"]["L"])) {
    for (unsigned long r = 0; r < _gridSize; ++r) {
        for (unsigned long c = 0; c < _gridSize; ++c) {
            this->G1time(r, c) = json["CYCLES"]["t_G1"][r][c];
            this->Stime(r, c) = json["CYCLES"]["t_S"][r][c];
            this->G2time(r, c) = json["CYCLES"]["t_G2"][r][c];
            this->Mtime(r, c) = json["CYCLES"]["t_M"][r][c];
            this->Dtime(r, c) = json["CYCLES"]["t_D"][r][c];
        }
    }
}

single_p& Cycles::G1time(unsigned long r, unsigned long c) {
	return _G1time[r * _gridSize + c];
}

single_p& Cycles::Stime(unsigned long r, unsigned long c) {
	return _Stime[r * _gridSize + c];
}

single_p& Cycles::G2time(unsigned long r, unsigned long c) {
	return _G2time[r * _gridSize + c];
}

single_p& Cycles::Mtime(unsigned long r, unsigned long c) {
	return _Mtime[r * _gridSize + c];
}

single_p& Cycles::Dtime(unsigned long r, unsigned long c) {
	return _Dtime[r * _gridSize + c];
}

std::ofstream& operator<<(std::ofstream& stream, const Cycles& cycles) {
	stream << cycles._G1time << cycles._Stime << cycles._G2time << cycles._Mtime << cycles._Dtime;
	return stream;
}