#pragma once
#include <memory>
#include "AbstrOptimizer.h"

class NelderMeadOptimizer : public AbstrOptimizer {
public:
	NelderMeadOptimizer(double alpha, double beta, double gamma) : alpha(alpha), beta(beta), gamma(gamma) {}
	optimResult optimize(std::shared_ptr<const AbstrFunction> f, const Point& init,
		const RectArea& area, std::shared_ptr<const AbstrStopCriteria> stop_criteria) override;
    std::string name() const override {
        return "Nelder-Mead simplex method";
    }

private:
	double alpha;
	double beta;
	double gamma;
};

