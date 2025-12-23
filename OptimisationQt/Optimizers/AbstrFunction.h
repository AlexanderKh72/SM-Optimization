#pragma once
#include <vector>
#include <string>

/// Математическая функция
/**
 * @brief AbstrFunction --- абстрактный класс позволяющий вычислять значение многомерной числовой функции.
 */
class AbstrFunction {
public:
	virtual double operator()(const std::vector<double>& x) const = 0;
    /**< Оператор AbstrFunction::operator() вычисляет значение функции в точке std::vector<double> x. */
	virtual size_t getDim() const = 0;
    /**< Функция AbstrFunction::getDim возвращает размерность функции, т.е. необходимую длину вектора для вычисления значения. */
    virtual std::string name() const = 0;
    /**< Функция Abstr::name возвращает имя функции. */
};

/// Функция Розенброка
/**
 * @brief Rosenbrock<n> --- шаблон класса, реализующего функцию Розенброка размерности n.
 */
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
    std::string name() const override {
        return "Rosenbrock";
    }
};

/// Сферическая функция
/**
 * @brief Rosenbrock<n> --- шаблон класса, реализующего сферическую функцию размерности n.
 */
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
    std::string name() const override {
        return "Sphere";
    }
};
