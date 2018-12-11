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
    for (unsigned long i = 0; i < _gridSize; ++i) {
        for (unsigned long j = 0; j < _gridSize; ++j) {
            this->G1time(i, j) = json["CYCLES"]["t_G1"][j][i];
            this->Stime(i, j) = json["CYCLES"]["t_S"][j][i];
            this->G2time(i, j) = json["CYCLES"]["t_G2"][j][i];
            this->Mtime(i, j) = json["CYCLES"]["t_M"][j][i];
            this->Dtime(i, j) = json["CYCLES"]["t_D"][j][i];
        }
    }
}

double& Cycles::G1time(unsigned long x, unsigned long y) {
	return _G1time[y * _gridSize + x];
}

double& Cycles::Stime(unsigned long x, unsigned long y) {
	return _Stime[y * _gridSize + x];
}

double& Cycles::G2time(unsigned long x, unsigned long y) {
	return _G2time[y * _gridSize + x];
}

double& Cycles::Mtime(unsigned long x, unsigned long y) {
	return _Mtime[y * _gridSize + x];
}

double& Cycles::Dtime(unsigned long x, unsigned long y) {
	return _Dtime[y * _gridSize + x];
}
