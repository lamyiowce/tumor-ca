#ifndef TUMOR_CYCLES_H
#define TUMOR_CYCLES_H

#include <vector>

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
	Cycles(std::vector<double> G1time,
		   std::vector<double> Stime,
		   std::vector<double> G2time,
		   std::vector<double> Mtime,
		   std::vector<double> Dtime);
	Cycles(Cycles&) = default;

	double& G1time(unsigned long x, unsigned long y);
	double& Stime(unsigned long x, unsigned long y);
	double& G2time(unsigned long x, unsigned long y);
	double& Mtime(unsigned long x, unsigned long y);
	double& Dtime(unsigned long x, unsigned long y);
};

#endif  // TUMOR_CYCLES_H
