#pragma once
#include <memory>
#include "AbstrOptimizer.h"

class SimpleStochasticOptimizer : public AbstrOptimizer {
public:
	SimpleStochasticOptimizer(double p, double delta) : p(p), delta(delta) {}

	optimResult optimize(std::shared_ptr<const AbstrFunction> f, const Point& init,
		const RectArea& area, std::shared_ptr<const AbstrStopCriteria> stop_criteria) override;

protected: 
	double p;
	double delta;
};
