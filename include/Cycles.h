#ifndef TUMOR_CYCLES_H
#define TUMOR_CYCLES_H

#include <vector>
#include <json.hpp>

class Cycles {
private:
	std::vector<double> _G1time;
	std::vector<double> _Stime;
	std::vector<double> _G2time;
	std::vector<double> _Mtime;
	std::vector<double> _Dtime;
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
};

#endif  // TUMOR_CYCLES_H
