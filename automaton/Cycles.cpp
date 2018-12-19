#include <Cycles.h>

Cycles::Cycles(unsigned long gridSize) :_gridSize(gridSize) {
	this->_G1time = std::vector<double>(_gridSize * _gridSize);
	this->_Stime = std::vector<double>(_gridSize * _gridSize);
	this->_G2time = std::vector<double>(_gridSize * _gridSize);
	this->_Mtime = std::vector<double>(_gridSize * _gridSize);
	this->_Dtime = std::vector<double>(_gridSize * _gridSize);
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

double& Cycles::G1time(unsigned long r, unsigned long c) {
	return _G1time[r * _gridSize + c];
}

double& Cycles::Stime(unsigned long r, unsigned long c) {
	return _Stime[r * _gridSize + c];
}

double& Cycles::G2time(unsigned long r, unsigned long c) {
	return _G2time[r * _gridSize + c];
}

double& Cycles::Mtime(unsigned long r, unsigned long c) {
	return _Mtime[r * _gridSize + c];
}

double& Cycles::Dtime(unsigned long r, unsigned long c) {
	return _Dtime[r * _gridSize + c];
}
