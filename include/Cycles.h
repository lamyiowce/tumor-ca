#ifndef TUMOR_CYCLES_H
#define TUMOR_CYCLES_H

#include <vector>
#include <json.hpp>

class Cycles {
private:
	std::vector<float> _G1time;
	std::vector<float> _Stime;
	std::vector<float> _G2time;
	std::vector<float> _Mtime;
	std::vector<float> _Dtime;

public:
	explicit Cycles(unsigned long gridSize);
	explicit Cycles(nlohmann::json json);
	Cycles(const Cycles&) = default;

	float& G1time(unsigned long r, unsigned long c);
	float& Stime(unsigned long r, unsigned long c);
	float& G2time(unsigned long r, unsigned long c);
	float& Mtime(unsigned long r, unsigned long c);
	float& Dtime(unsigned long r, unsigned long c);

	const float& G1time(unsigned long r, unsigned long c) const;
	const float& Stime(unsigned long r, unsigned long c) const;
	const float& G2time(unsigned long r, unsigned long c) const;
	const float& Mtime(unsigned long r, unsigned long c) const;
	const float& Dtime(unsigned long r, unsigned long c) const;

	unsigned long gridSize;
};

#endif  // TUMOR_CYCLES_H
