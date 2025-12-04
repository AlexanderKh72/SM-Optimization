#include <random>
#include <iostream>
#include "NelderMeadOptimizer.h"

Point mixture(const Point& x1, const Point& x2, double alpha) {
	std::vector<double> result;
	// TODO: consistent dimensions
	for (size_t i = 0; i < (x1.getDim()); ++i)
		result.push_back(alpha * x1.at(i) + (1 - alpha) * x2.at(i));
	Point result_point(result);
	return result_point;
}

Point bound_intersection(const Point& x0, const Point& x1, const RectArea& area) {
	// TODO: consistent dimenesions
	double alpha = 0;
	for (size_t i = 0; i < x0.getDim(); ++i) {
		if (x1.at(i) < area.get_l(i))
			if ((area.get_l(i) - x1.at(i)) / (x0.at(i) - x1.at(i)) > alpha)
				alpha = (area.get_l(i) - x1.at(i)) / (x0.at(i) - x1.at(i));
		if (x1.at(i) > area.get_r(i))
			if ((x1.at(i) - area.get_r(i)) / (x1.at(i) - x0.at(i)) > alpha)
				alpha = (area.get_l(i) - x1.at(i)) / (x0.at(i) - x1.at(i));
	}
	return mixture(x0, x1, alpha);
}

optimResult NelderMeadOptimizer::optimize(const AbstrFunction * f, 
	const Point& init,
	const RectArea& area, const AbstrStopCriteria * stop_criteria)
{
	size_t n = f->getDim();
	std::vector<Point> simplex;
	std::vector<double> f_simplex;
	size_t h = 0, s = 0, l = 0;
	trajectory tr;

	simplex.push_back(init);
	f_simplex.push_back((*f)(init));
	double t = area.get_r(0) - simplex[0].at(0);
	for (int i = 0; i < n; ++i)
		if (t > area.get_r(i) - simplex[0].at(i))
			t = area.get_r(i) - simplex[0].at(i);
	double d1 = t / (n * sqrt(2)) * (sqrt(n + 1) + n - 1);
	double d2 = t / (n * sqrt(2)) * (sqrt(n + 1) - 1);

	for (size_t i = 1; i < n + 1; ++i) {
		std::vector<double> p;
		p.resize(n);
		for (int j = 0; j < n; ++j) {
			if (j == i)
				p[j] = simplex[0].at(j) + d1;
			else
				p[j] = simplex[0].at(j) + d2;
		}
		simplex.push_back(std::move(Point(p)));
		f_simplex.push_back((*f)(p));
	}

	tr.N = 0;
	while (!stop_criteria->IsSatisfied(tr)) {
		// (i)
		l = h = s = 0;
		for (size_t i = 1; i < n + 1; ++i) {
			if (f_simplex[i] < f_simplex[l])
				l = i;
			if (f_simplex[i] > f_simplex[h]) 
				h = i;
		}
		s = l;
		for (size_t i = 0; i < n + 1; ++i) {
			if (i != h && f_simplex[i] > f_simplex[s])
				s = i;
		}
		std::vector<double> x0_coord;
		for (size_t j = 0; j < n; ++j) {
			double xj = 0;
			for (int i = 0; i < n + 1; ++i) {
				if (i != h) xj += simplex[i].at(j);
			}
			x0_coord.push_back(xj / n);
		}
		Point x0(x0_coord);
		++tr.N;
		tr.xs.push_back(simplex[h]);
		tr.fs.push_back(f_simplex[h]);
		//std::cout << "Step " << tr.N << std::endl;
		//std::cout << "Simplex:\n";
		//for (size_t i = 0; i < simplex.size(); ++i) {
		//	simplex.at(i).print();
		//	std::cout << " " << f_simplex.at(i) << std::endl;
		//}
		//std::cout << "Centroid:\n";
		//x0.print();
		//std::cout << " " << f(x0) << std::endl;
		// (ii)
		Point x_r(std::move(mixture(x0, simplex[h], 1 + alpha)));
		x_r = bound_intersection(x0, x_r, area);
		double f_r = (*f)(x_r);
		//std::cout << "Reflection:\n";
		//x_r.print();
		//std::cout << " " << f_r << std::endl;
		// (iii)
 		if (f_simplex[s] >= f_r && f_r >= f_simplex[l]) {
			// reform simplex
			simplex[h] = x_r;
			f_simplex[h] = f_r;
		}
		// (iv)
		else if (f_r < f_simplex[l]) {
			Point x_e = mixture(x_r, x0, gamma);
			x_e = bound_intersection(x0, mixture(x_r, x0, gamma), area);
			double f_e = (*f)(x_e);
			//std::cout << "Expansion:\n";
			//x_e.print();
			//std::cout << " " << f_e << std::endl;
			if (f_e < f_simplex[l]) {
				// reform simplex
				simplex[h] = x_e;
				f_simplex[h] = f_e;
			}
			else {
				// reform simplex
				simplex[h] = x_r;
				f_simplex[h] = f_r;
			}
		}
		// (v)
		else { // if (f_r > f_simplex[s])
			if (f_r < f_simplex[h]) {
				simplex[h] = x_r;
				f_simplex[h] = f_r;
			}

			Point x_c = mixture(simplex[h], x0, beta);
			double f_c = (*f)(x_c);
			//std::cout << "Contraction:\n";
			//x_c.print();
			//std::cout << " " << f_c << std::endl;
			if (f_simplex[h] > f_c) {
				simplex[h] = x_c;
				f_simplex[h] = f_c;
			}
			else {
				//std::cout << "Shrinking\n";
				for (size_t i = 0; i < n + 1; ++i) {
					if (i != l) {
						simplex[i] = mixture(simplex[l], simplex[i], 0.5);
						f_simplex[i] = (*f)(simplex[i]);
					}
				}
			}
		}
	}
	l = 0; double f_l = f_simplex[0];
	for (size_t i = 1; i < n + 1; ++i) {
		if (f_simplex[i] < f_l) {
			l = i;
			f_l = f_simplex[i];
		}
	}
	return optimResult{ simplex.at(l), f_l, tr };
}
