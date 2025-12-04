#include "SimpleStochasticOptimizer.h"
#include <random>
#include <vector>
#include <algorithm>

optimResult SimpleStochasticOptimizer::optimize(std::shared_ptr<const AbstrFunction> f, 
    const Point& init, 
    const RectArea& area, std::shared_ptr<const AbstrStopCriteria> stop_criteria)
{
    Point x_curr(init);
    double f_curr = (*f)(x_curr);
    trajectory tr;
    tr.xs.push_back(init);
    tr.fs.push_back((*f)(tr.xs[0]));

    std::mt19937 gen(42);
    std::bernoulli_distribution local_cond(p);
    std::vector<std::uniform_real_distribution<double>> new_global_point;
    for (size_t i = 0; i < f->getDim(); ++i) {
        new_global_point.push_back(std::uniform_real_distribution<double>(area.get_l(i), area.get_r(i)));
    }
    tr.N = 0;
    while (!stop_criteria->IsSatisfied(tr)) {
        ++tr.N;
        std::vector<double> coord;
        if (local_cond(gen))
            for (size_t i = 0; i < f->getDim(); ++i)
                coord.push_back(std::uniform_real_distribution<double>(
                    std::max(x_curr.at(i) - delta, area.get_l(i)),
                    std::min(x_curr.at(i) + delta, area.get_r(i)))(gen));
        else
            for (size_t i = 0; i < f->getDim(); ++i)
                coord.push_back(new_global_point[i](gen));
        Point x_new(coord);
        double f_new = (*f)(x_new);
        if (f_new < f_curr) {
            tr.xs.push_back(x_new);
            tr.fs.push_back(f_new);
            x_curr = std::move(x_new);
            f_curr = f_new;
        }
    }

    return optimResult{tr.xs.back(), tr.fs.back(), tr};
}
