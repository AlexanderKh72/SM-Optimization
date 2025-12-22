#include "Point.h"
#include <iostream>

Point::Point(Point&& other) {
	std::swap(coord, other.coord);
	std::swap(dim, other.dim);
}

Point& Point::operator=(const Point& other) {
	if (this == &other) return (*this);
	coord = other.coord;
	dim = other.dim;
	return (*this);
}

Point& Point::operator=(Point&& other) {
	if (this == &other) return (*this);
	coord = std::move(other.coord);
	dim = std::move(other.dim);
	return (*this);
}

void Point::print() const {
	std::cout << "{ ";
	for (size_t i = 0; i < dim; ++i)
		std::cout << coord[i] << " ";
	std::cout << "}";
}
