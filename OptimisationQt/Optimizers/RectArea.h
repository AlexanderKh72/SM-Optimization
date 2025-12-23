#pragma once
#include <vector>
#include "Point.h"

/// Прямоугольная область
/**
 * @brief RectArea --- класс, релизующий параллелепипедную область в многомерном пространстве (коробка).
 */
class RectArea {
public:
	// TODO: consistent dimensions
	RectArea(const std::vector<double>& l, const std::vector<double>& r) : left_bound(l), right_bound(r) {}
    /**< Конструктор принимающий на вход const std::vector<double>& l --- левые границы по каждой координате для точек области,
     * const std::vector<double>& r --- соответственно, правые границы по каждой координате для точек области.
    */
    explicit RectArea(double x1 = 0, double y1 = 0, double x2 = 1, double y2 = 1)  : left_bound({x1, y1}), right_bound({x2, y2})  {}
    /**< Конструктор принимающий на вход четыре числа x1, y1, x2, y2 и создающий прямоугольную область [x1, x2] x [y1, y2]. */
	size_t getDim() const { return left_bound.getDim(); }
    /** Функция RectArea::getDim возвращает размерность пространства, в котором лежит область. */
	double get_l(size_t i) const { return left_bound.at(i); }
    /** Функция RectArea::get_l возвращает левую границу для i-ой координаты точки в области.  */
	double get_r(size_t i) const { return right_bound.at(i); }
    /** Функция RectArea::get_r возвращает правую границу для i-ой координаты точки в области.  */
	
protected:
	const Point left_bound;
	const Point right_bound;
};
