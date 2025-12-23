#pragma once
#include "AbstrFunction.h"
#include "RectArea.h"
#include "Point.h"
#include <memory>
#include <cmath>

/// Траектория оптимизации
/**
 * @brief trajectory --- структура, хранящая последовательность (std::vector) точек типа Point, значений функции в этих точках и число итераций оптимизатора.
 */
struct trajectory {
	std::vector<Point> xs;
    /**< trajectory::xs --- последовательность типа  std::vector<Point>, элементы которой --- точки посчитанные на кажом шаге итерации оптимизатора.  */
	std::vector<double> fs;
    /**< trajectory::fs --- последовательность типа  std::vector<double>, элементы которой --- значения функции в точках из trajectory::xs.  */
	size_t N;
    /**< trajectory::N --- число итераций.  */

	trajectory() : N(0) {}

	trajectory(trajectory&& other) noexcept : xs(std::move(other.xs)), fs(std::move(other.fs)), N(other.N) {}
	trajectory(const trajectory& other) : xs(other.xs), fs(other.fs), N(other.N) {}

	trajectory operator=(trajectory&& other) noexcept {
		if (this == &other) return (*this);
		xs = std::move(other.xs);
		fs = std::move(other.fs);
		N = other.N;
		return (*this);
	}
};

/// Результат работы оптимизатора.
/**
 * @brief optimResult --- структура хранящая точку, найденную в результате работы оптимизатора, значение функции в этой точке и траекторию работы оптимизатора
 */
struct optimResult {
	Point x;
    /**< optimResult::x --- точка типа  Point, полученная на последнем шаге работы оптимизатора.  */
	double f;
    /**< optimResult::f --- значение функции в точке optimResult::x.  */
	trajectory tr;
    /**< optimResult::tr --- траектория работы оптимизатора типа trajectory.  */

	optimResult(const Point& x, double f, const trajectory& tr) : x(x), f(f), tr(tr) {}

	optimResult(optimResult&& other) noexcept : x(std::move(other.x)), f(other.f), tr(std::move(other.tr)) {}
	optimResult(const optimResult& other) : x(other.x), f(other.f), tr(other.tr)  {}

	optimResult operator=(optimResult&& other) noexcept {
		if (this == &other) return (*this);
		x = std::move(other.x);
		f = other.f;
		tr = std::move(other.tr);
		return (*this);
	}
};

/// Критерий остановки
/**
 * @brief AbstrStopCriteria --- абстрактный класс? реализующий критерий остановки оптимизатора.
 */
class AbstrStopCriteria {
public:
	virtual bool IsSatisfied(const trajectory& t) const = 0;
    /**< Функция AbstrStopCriteria::IsSatisfied принимает на вход траекторию работы оптимизатора типа trajectory и возвращает true если критерий удовлетворён и false --- иначе. */
};

/// Критерий остановки при малом относительном убывании функции
/**
 * @brief SC --- класс, реализующий критерий остановки оптимизатора, когда абсолютный прирост функции относительно значения функции становится мал.
 */
class SC : public AbstrStopCriteria {
public:
    SC(double epsilon, size_t N) : epsilon(epsilon), N_max(N) {}
    /**< Конструктор принимающий на вход два параметра критерия остановки:
     * epsilon --- точность,
     * N --- максимальное число итераций.
     */
    bool IsSatisfied(const trajectory& t) const override {
        return (t.N > 1 && t.fs.size() > 1) && (std::abs((t.fs.at(t.fs.size() - 1) - t.fs.at(t.fs.size() - 2)) / t.fs.at(t.fs.size() - 1)) < epsilon) || (t.N > N_max);
    }
     /**< Функция SC::IsSatisfied принимает на вход траекторию работы оптимизатора типа trajectory и возвращает true если критерий удовлетворён и false --- иначе.
      * Критерий удовлетворён, если число итераций и длина траектории больше 1, а значение (f_{n-1} - f_{n-2}) / f_{n-1} меньше epsilon (n --- длина траектории, f_i --- значение функции на i-ом шаге оптимизатора),
      * или число итераций превысило N.
      */
protected:
    double epsilon;
    size_t N_max;
};

/// Оптимизатор
/**
 * @brief AbstrOptimizer --- абстрактный класс, реализующий оптимизатор.
 */
class AbstrOptimizer {
public:
	virtual optimResult optimize(std::shared_ptr<const AbstrFunction> f, const Point& init,
		const RectArea& area, std::shared_ptr<const AbstrStopCriteria> stop_criteria) = 0;
    /**< Функция AbstrOptimizer::optimize принимает на вход
     * указатель на функцию типа std::shared_ptr<const AbstrFunction>,
     * начальное приближение типа Point,
     * прямоугольную область, в которой ведётся поиск минимума, типа RectArea,
     * укащатель на критерий остановки типа std::shared_ptr<const AbstrStopCriteria>.
     *
     * Возвращает результат работы метода в переменной типа optimResult.
     */
    virtual std::string name() const = 0;
    /**< Функция AbstrOptimizer::name возвращает название метода оптимизации. */
};
