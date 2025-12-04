#pragma once
#include <vector>
#include "Point.h"

class RectArea {
public:
	// TODO: consistent dimensions
	RectArea(const std::vector<double>& l, const std::vector<double>& r) : left_bound(l), right_bound(r) {}
	size_t getDim() const { return left_bound.getDim(); }
	double get_l(size_t i) const { return left_bound.at(i); }
	double get_r(size_t i) const { return right_bound.at(i); }
	
protected:
	const Point left_bound;
	const Point right_bound;
};
