#pragma once
#include <memory>
#include "AbstrOptimizer.h"

/// Метод простой стохастичиской оптимизации
/**
 * @brief SimpleStochasticOptimizer --- класс, реализующий метод простого стохастического поиска минимума функции.
 */
class SimpleStochasticOptimizer : public AbstrOptimizer {
public:
	SimpleStochasticOptimizer(double p, double delta) : p(p), delta(delta) {}

	optimResult optimize(std::shared_ptr<const AbstrFunction> f, const Point& init,
		const RectArea& area, std::shared_ptr<const AbstrStopCriteria> stop_criteria) override;
    std::string name() const override {
        return "Simple stochastic method";
    }

protected: 
	double p;
	double delta;
};
