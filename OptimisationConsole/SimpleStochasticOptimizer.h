#pragma once
#include "AbstrOptimizer.h"

class SimpleStochasticOptimizer : public AbstrOptimizer {
public:
	SimpleStochasticOptimizer(double p, double delta) : p(p), delta(delta) {}

	optimResult optimize(const AbstrFunction * f, const Point& init,
		const RectArea& area, const AbstrStopCriteria * stop_criteria) override;

protected: 
	double p;
	double delta;
};
