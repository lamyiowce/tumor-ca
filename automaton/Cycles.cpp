#include <Cycles.h>

Cycles::Cycles(unsigned long gridSize) :gridSize(gridSize) {
	this->_G1time = std::vector<double>(gridSize * gridSize);
	this->_Stime = std::vector<double>(gridSize * gridSize);
	this->_G2time = std::vector<double>(gridSize * gridSize);
	this->_Mtime = std::vector<double>(gridSize * gridSize);
	this->_Dtime = std::vector<double>(gridSize * gridSize);
}

Cycles::Cycles(nlohmann::json json)
    : Cycles::Cycles(static_cast<unsigned long>(json["PARAMS"]["L"])) {
    for (unsigned long r = 0; r < gridSize; ++r) {
        for (unsigned long c = 0; c < gridSize; ++c) {
            this->G1time(r, c) = json["CYCLES"]["t_G1"][r][c];
            this->Stime(r, c) = json["CYCLES"]["t_S"][r][c];
            this->G2time(r, c) = json["CYCLES"]["t_G2"][r][c];
            this->Mtime(r, c) = json["CYCLES"]["t_M"][r][c];
            this->Dtime(r, c) = json["CYCLES"]["t_D"][r][c];
        }
    }
}

double& Cycles::G1time(unsigned long r, unsigned long c) {
	return _G1time[r * gridSize + c];
}

double& Cycles::Stime(unsigned long r, unsigned long c) {
	return _Stime[r * gridSize + c];
}

double& Cycles::G2time(unsigned long r, unsigned long c) {
	return _G2time[r * gridSize + c];
}

double& Cycles::Mtime(unsigned long r, unsigned long c) {
	return _Mtime[r * gridSize + c];
}

double& Cycles::Dtime(unsigned long r, unsigned long c) {
	return _Dtime[r * gridSize + c];
}

const double &Cycles::G1time(unsigned long r, unsigned long c) const {
	return _G1time[r * gridSize + c];
}

const double& Cycles::Stime(unsigned long r, unsigned long c) const {
	return _Stime[r * gridSize + c];
}

const double& Cycles::G2time(unsigned long r, unsigned long c) const {
	return _G2time[r * gridSize + c];
}

const double& Cycles::Mtime(unsigned long r, unsigned long c) const {
	return _Mtime[r * gridSize + c];
}

const double& Cycles::Dtime(unsigned long r, unsigned long c) const {
	return _Dtime[r * gridSize + c];
}