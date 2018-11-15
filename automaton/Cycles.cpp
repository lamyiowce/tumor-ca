#include <Cycles.h>

Cycles::Cycles(unsigned long gridSize) :_gridSize(gridSize) {
	this->_G1time = std::vector<double>(_gridSize * _gridSize);
	this->_Stime = std::vector<double>(_gridSize * _gridSize);
	this->_G2time = std::vector<double>(_gridSize * _gridSize);
	this->_Mtime = std::vector<double>(_gridSize * _gridSize);
	this->_Dtime = std::vector<double>(_gridSize * _gridSize);
}

Cycles::Cycles(std::vector<double> G1time,
			   std::vector<double> Stime,
			   std::vector<double> G2time,
			   std::vector<double> Mtime,
			   std::vector<double> Dtime)
	: _G1time(G1time),
	  _Stime(Stime),
	  _G2time(G2time),
	  _Mtime(Mtime),
	  _Dtime(Dtime),
	  _gridSize(G1time.size()) {}

double& Cycles::G1time(unsigned long x, unsigned long y) {
	return _G1time[y * _gridSize + x];
}

double& Cycles::Stime(unsigned long x, unsigned long y) {
	return _G1time[y * _gridSize + x];
}

double& Cycles::G2time(unsigned long x, unsigned long y) {
	return _G1time[y * _gridSize + x];
}

double& Cycles::Mtime(unsigned long x, unsigned long y) {
	return _G1time[y * _gridSize + x];
}

double& Cycles::Dtime(unsigned long x, unsigned long y) {
	return _G1time[y * _gridSize + x];
}
