#pragma once
#include "AbstrOptimizer.h"

class NelderMeadOptimizer : public AbstrOptimizer {
public:
	NelderMeadOptimizer(double alpha, double beta, double gamma) : alpha(alpha), beta(beta), gamma(gamma) {}
	optimResult optimize(const AbstrFunction * f, const Point& init,
		const RectArea& area, const AbstrStopCriteria * stop_criteria) override;

private:
	double alpha;
	double beta;
	double gamma;
};

