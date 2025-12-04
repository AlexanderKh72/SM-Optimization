// OptimisationConsole.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "AbstrFunction.h"
#include "NelderMeadOptimizer.h"
#include "SimpleStochasticOptimizer.h"
#include "RectArea.h"

template<size_t n = 2> 
class Rosenbrock : public AbstrFunction {
public:
    size_t getDim() const override { return n; }
    double operator()(const std::vector<double>& x) const override {
        double res = 0;
        for (size_t i = 0; i < n - 1; ++i) {
            res += 100 * (x[i + 1] - x[i] * x[i]) * (x[i + 1] - x[i] * x[i]) + (1 - x[i]) * (1 - x[i]);
        }
        return res;
    }
};

template<size_t n = 2>
class Sphere : public AbstrFunction {
public:
    size_t getDim() const override { return n; }
    double operator()(const std::vector<double>& x) const override {
        double res = 0;
        for (size_t i = 0; i < n; ++i) {
            res += x.at(i) * x.at(i);
        }
        return res;
    }
};

class SC : public AbstrStopCriteria {
public:
    SC(double epsilon, size_t N) : epsilon(epsilon), N_max(N) {}
    bool IsSatisfied(const trajectory& t) const override {
        return (t.N > 1 && t.fs.size() > 1) && (abs((t.fs.at(t.fs.size() - 1) - t.fs.at(t.fs.size() - 2)) / t.fs.at(t.fs.size() - 1)) < epsilon) || (t.N > N_max);
    }
protected:
    double epsilon;
    size_t N_max;
};

int main()
{
    //SC stopcriteria(0.001);
    //std::vector<double> l = { -1.5, -1.5 }, r = { 1.5, 1.5 };
    //RectArea area(l, r);
    //NelderMeadOptimizer optimizer_NM;
    //Rosenbrock<2> f;
    //optimResult res = optimizer_NM.optimize(f, Point(std::vector<double>{0.5, 0.5}), area, stopcriteria, std::vector<double>{1, 0.5, 2});
    //std::cout << res.tr.N << std::endl;
    //for (int i = 0; i < res.x.getDim(); ++i) {
    //    std::cout << res.x.at(i) << " ";
    //}
    //std::cout << "\n" << res.f << std::endl;

    //SimpleStochasticOptimizer optimizer_SS;
    //res = std::move(optimizer_SS.optimize(f, Point(std::vector<double>{0.5, 0.5}), area, stopcriteria, std::vector<double>{0.5, 0.2}));
    //std::cout << res.tr.N << std::endl;
    //for (int i = 0; i < res.x.getDim(); ++i) {
    //    std::cout << res.x.at(i) << " ";
    //}
    //std::cout << "\n" << res.f << std::endl;
        const size_t n = 2;
    AbstrFunction* f = nullptr;
    AbstrOptimizer* optimizer = nullptr;
    std::vector<double> area_left_bound, area_right_bound;
    AbstrStopCriteria* stopcr = nullptr;

    char c;
    
    std::cout << "Dimensions:\nn = " << n;
    
    while (!f) {
        std::cout << "\nSelect function:\n1 -- Rosenbrock;\n2 -- Sphere function;\n";
        std::cin >> c;
        if (c == '1')
            f = new (Rosenbrock<n>);
        else if (c == '2')
            f = new (Sphere<n>);
        else
            std::cout << "Try again.\n";
    }

    std::cout << "\nEnter box area boundries.\nFor dimension i, pair (x_i, y_i), such as x_i < y_i, is boundries.\nYour input should look like:\nx_1 y_1\nx_2 y_2\n...\nx_n y_n\n";
    std::cout << "Enter boundries:\n";
    for (size_t i = 0; i < n; ++i) {
        double x, y;
        std::cin >> x >> y;
        while (x >= y) {
            std::cout << "Error: x_"<<i<<" >= y_"<<i<<". Try again.\n";
            std::cin >> x >> y;
        }
        area_left_bound.push_back(x);
        area_right_bound.push_back(y);
    }
    RectArea area(area_left_bound, area_right_bound);

    std::cout << "\nEnter initial point coordinates:\n";
    std::vector<double> init_coord;
    for (size_t i = 0; i < n; ++i) {
        double x;
        std::cin >> x;
        while (x < area.get_l(i) || x > area.get_r(i)) {
            std::cout << "Error: init. point out of area (coord. " << i << "). Try again.\n";
            std::cin >> x;
        }
        init_coord.push_back(x);
    }
    Point init(init_coord);

    while (!optimizer) {
        std::cout << "\nSelect optimizing method:\n1 -- Nelder-Mead optimizer;\n2 -- Simple stochastic optimizer\n";
        std::cin >> c;
        if (c == '1') {
            double alpha, beta, gamma;
            std::cout << "Enter parameters:\nalpha, such as alpha > 0;\nbeta, such as 0 < beta < 1;\ngamma, such as gamma > 1.\nRecommended: alpha=1, beta=0.5, gamma=2.\n";
            std::cout << "alpha = ";
            std::cin >> alpha;
            while (alpha <= 0) {
                std::cout << "Error alpha <= 0. Try again.\nalpha = ";
                std::cin >> alpha;
            }
            std::cout << "beta = ";
            std::cin >> beta;
            while (beta <= 0 || beta >= 1) {
                std::cout << "Error beta <= 0 or beta >= 1. Try again.\nbeta = ";
                std::cin >> beta;
            }
            std::cout << "gamma = ";
            std::cin >> gamma;
            while (gamma <= 1) {
                std::cout << "Error gamma <= 1. Try again.\ngamma = ";
                std::cin >> gamma;
            }

            optimizer = new NelderMeadOptimizer(alpha, beta, gamma);
        }
        else if (c == '2') {
            double p, delta;
            std::cout << "Enter parameters:\ndelta, such as delta > 0;\np, such as 0 < p < 1l\n";
            std::cout << "delta = ";
            std::cin >> delta;
            while (delta <= 0) {
                std::cout << "Error delta <= 0. Try again.\ndelta = ";
                std::cin >> delta;
            }
            std::cout << "p = ";
            std::cin >> p;
            while (p <= 0 || p >= 1) {
                std::cout << "Error p <= 0 or beta >= 1. Try again.\np = ";
                std::cin >> p;
            }

            optimizer = new SimpleStochasticOptimizer(p, delta);
        }
        else
            std::cout << "Try again.\n";
    }
    
    double epsilon;
    size_t N_max;
    std::cout << "\nEnter parameters of stop criteria epsilon and N_max:\n |( f(x_n) - f(x_{n-1}) ) / f(x_n)| < epsilon, max. num. of iterations is N_max.\n";
    std::cout << "epsilon = 0, means that stop criteria is number of iterations is equal to N_max\n";
    std::cout << "epsilon = ";
    std::cin >> epsilon;
    while (epsilon < 0) {
        std::cout << "Error: epsilon < 0. Try again.";
        std::cout << "epsilon = ";
        std::cin >> epsilon;
    }
    std::cout << "N_max = ";
    std::cin >> N_max;
    stopcr = new SC(epsilon, N_max);

    optimResult res(optimizer->optimize(f, init, area, stopcr));
    std::cout << "\n----------\n\nResult.\n";
    std::cout << "Found point: ";
    res.x.print();
    std::cout << "\nFunction value: " << res.f << "\n";
    std::cout << "Number of iterations: " << res.tr.N;

    delete optimizer;
    delete f;
    delete stopcr;
}
