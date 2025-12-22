#pragma once
#include <vector>

class Point {
public:
	Point() : dim(0) {}
	Point(const std::vector<double>& x)  : dim(x.size()), coord(x) {}
	Point(const Point& other) : dim(other.dim) , coord(other.coord) {}
	Point(Point&& other);
	size_t getDim() const { return dim; }
	Point& operator=(const Point& other) ;
	Point& operator=(Point&& other) ;
	double at(size_t i) const { return coord.at(i); }
	operator std::vector<double>() const { return coord; }

	void print() const;
private:
	size_t dim;
	std::vector<double> coord;
};
