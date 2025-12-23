#pragma once
#include <vector>

/// Точка в многомерном пространстве.
/**
 * @brief Point --- класс, реализующий точку в многомерном пространстве.
 */
class Point {
public:
	Point() : dim(0) {}
	Point(const std::vector<double>& x)  : dim(x.size()), coord(x) {}
    /**< Конструктор принимающий на вход координаты точки const std::vector<double>& x. */
	Point(const Point& other) : dim(other.dim) , coord(other.coord) {}
	Point(Point&& other);
	size_t getDim() const { return dim; }
    /**< Функция Point::getDim возвращает размерность пространства, в котором лежит точка. */
	Point& operator=(const Point& other) ;
	Point& operator=(Point&& other) ;
	double at(size_t i) const { return coord.at(i); }
    /**< Функция Point::at возвращает i-ую координату точки. */
	operator std::vector<double>() const { return coord; }

	void print() const;
private:
	size_t dim;
	std::vector<double> coord;
};
