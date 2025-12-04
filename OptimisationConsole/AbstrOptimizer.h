#pragma once
#include "AbstrFunction.h"
#include "RectArea.h"
#include "Point.h"

struct trajectory {
	std::vector<Point> xs;
	std::vector<double> fs;
	size_t N;

	trajectory() : N(0) {}

	trajectory(trajectory&& other) noexcept : xs(std::move(other.xs)), fs(std::move(other.fs)), N(other.N) {}
	trajectory(const trajectory& other) : xs(other.xs), fs(other.fs), N(other.N) {}

	trajectory operator=(trajectory&& other) noexcept {
		if (this == &other) return (*this);
		xs = std::move(other.xs);
		fs = std::move(other.fs);
		N = other.N;
		return (*this);
	}
};

struct optimResult {
	Point x;
	double f;
	trajectory tr;

	optimResult(const Point& x, double f, const trajectory& tr) : x(x), f(f), tr(tr) {}

	optimResult(optimResult&& other) noexcept : x(std::move(other.x)), f(other.f), tr(std::move(other.tr)) {}
	optimResult(const optimResult& other) : x(other.x), f(other.f), tr(other.tr)  {}

	optimResult operator=(optimResult&& other) noexcept {
		if (this == &other) return (*this);
		x = std::move(other.x);
		f = other.f;
		tr = std::move(other.tr);
		return (*this);
	}
};

class AbstrStopCriteria {
public:
	virtual bool IsSatisfied(const trajectory& t) const = 0;
};

class AbstrOptimizer {
public:
	virtual optimResult optimize(const AbstrFunction* f, const Point& init,
		const RectArea& area, const AbstrStopCriteria* stop_criteria) = 0;
};
