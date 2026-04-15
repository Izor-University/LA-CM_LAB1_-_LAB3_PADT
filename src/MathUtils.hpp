#ifndef MATH_UTILS_HPP
#define MATH_UTILS_HPP

#include "Complex.hpp"

class MathUtils {
private:
    static double Sqrt(double n) {
        if (n <= 0.0) return 0.0;
        double x = n, y = 1.0, e = 0.000001;
        while (x - y > e) {
            x = (x + y) / 2.0;
            y = n / x;
        }
        return x;
    }

public:
    // 1. Обобщенный шаблон для ВСЕХ типов, поддерживающих оператор '<' и унарный минус
    template <class T>
    static double Abs(const T& val) {
        // Используем T() как эквивалент нуля для любого типа
        return val < T() ? static_cast<double>(-val) : static_cast<double>(val);
    }

    // 2. Явная перегрузка для Complex
    // Компилятор выберет её, так как конкретная функция имеет приоритет над шаблоном
    static double Abs(const Complex& c) {
        return Sqrt(c.re * c.re + c.im * c.im);
    }
};

#endif // MATH_UTILS_HPP