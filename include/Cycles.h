#ifndef TUMOR_CYCLES_H
#define TUMOR_CYCLES_H

#include <vector>
#include <json.hpp>
#include <util.h>

class Cycles {
private:
	std::vector<single_p> _G1time;
	std::vector<single_p> _Stime;
	std::vector<single_p> _G2time;
	std::vector<single_p> _Mtime;
	std::vector<single_p> _Dtime;
	unsigned long _gridSize;

public:
	explicit Cycles(unsigned long gridSize);
	explicit Cycles(nlohmann::json json);
	Cycles(const Cycles&) = default;

	single_p& G1time(unsigned long r, unsigned long c);
	single_p& Stime(unsigned long r, unsigned long c);
	single_p& G2time(unsigned long r, unsigned long c);
	single_p& Mtime(unsigned long r, unsigned long c);
	single_p& Dtime(unsigned long r, unsigned long c);

    const single_p& G1time(unsigned long r, unsigned long c) const;
    const single_p& Stime(unsigned long r, unsigned long c) const;
    const single_p& G2time(unsigned long r, unsigned long c) const;
    const single_p& Mtime(unsigned long r, unsigned long c) const;
    const single_p& Dtime(unsigned long r, unsigned long c) const;
};

#endif  // TUMOR_CYCLES_H
