#ifndef MATH_UTILS_HPP
#define MATH_UTILS_HPP

#include "Complex.hpp"

// Математические утилиты
class MathUtils {
private:
    // Метод Ньютона для вычисления квадратного корня
    static double Sqrt(double n) {
        if (n <= 0.0) {
            return 0.0;
        }
        double x = n;
        double y = 1.0;
        double e = 1e-15;

        while (x - y > e) {
            x = (x + y) / 2.0;
            y = n / x;
        }
        return x;
    }

public:
    // Модуль для обобщенных типов (поддерживающих унарный минус и сравнение)
    template <class T>
    static double Abs(const T& val) {
        if (val < T()) {
            return static_cast<double>(-val);
        }
        return static_cast<double>(val);
    }

    // Модуль для комплексных чисел
    static double Abs(const Complex& c) {
        return Sqrt(c.re * c.re + c.im * c.im);
    }
};

#endif // MATH_UTILS_HPP