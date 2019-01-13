#ifndef TUMOR_CYCLES_H
#define TUMOR_CYCLES_H

#include <vector>
#include <json.hpp>
#include <util.h>

class Cycles {
private:
	grid<double> _G1time;
	grid<double> _Stime;
	grid<double> _G2time;
	grid<double> _Mtime;
	grid<double> _Dtime;
	unsigned long _gridSize;

public:
	explicit Cycles(unsigned long gridSize);
	explicit Cycles(nlohmann::json json);
	Cycles(const Cycles&) = default;

	double& G1time(unsigned long r, unsigned long c);
	double& Stime(unsigned long r, unsigned long c);
	double& G2time(unsigned long r, unsigned long c);
	double& Mtime(unsigned long r, unsigned long c);
	double& Dtime(unsigned long r, unsigned long c);

	friend std::ofstream& operator<<(std::ofstream& stream, const Cycles& cycles);
};

#endif  // TUMOR_CYCLES_H
