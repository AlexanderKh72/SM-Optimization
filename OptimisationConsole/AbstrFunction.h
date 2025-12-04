#pragma once
#include <vector>

class AbstrFunction {
public:
	virtual double operator()(const std::vector<double>& x) const = 0;
	virtual size_t getDim() const = 0;
};
