#ifndef COMPLEX_HPP
#define COMPLEX_HPP

#include "Exceptions.hpp"

class Complex {
public:
    double re; // Действительная часть
    double im; // Мнимая часть

    // Конструктор по умолчанию позволяет неявно конвертировать double в Complex
    Complex(double r = 0.0, double i = 0.0) : re(r), im(i) {}

    // --- Перегрузка арифметических операторов ---
    Complex operator+(const Complex& other) const {
        return Complex(re + other.re, im + other.im);
    }
    Complex operator-(const Complex& other) const {
        return Complex(re - other.re, im - other.im);
    }
    Complex operator*(const Complex& other) const {
        return Complex(re * other.re - im * other.im, re * other.im + im * other.re);
    }
    Complex operator/(const Complex& other) const {
        double denom = other.re * other.re + other.im * other.im;
        if (denom == 0.0) throw MathError("Division by zero in complex numbers");
        return Complex((re * other.re + im * other.im) / denom,
                       (im * other.re - re * other.im) / denom);
    }

    // --- Исправленные операторы сравнения (с учетом машинной точности) ---
    bool operator==(const Complex& other) const {
        const double EPSILON = 1e-9; // Допустимая погрешность

        // Находим разницу
        double diffRe = re - other.re;
        double diffIm = im - other.im;

        // Берем модуль без использования внешних библиотек
        diffRe = diffRe < 0.0 ? -diffRe : diffRe;
        diffIm = diffIm < 0.0 ? -diffIm : diffIm;

        return (diffRe < EPSILON) && (diffIm < EPSILON);
    }

    bool operator!=(const Complex& other) const {
        return !(*this == other);
    }
};

#endif // COMPLEX_HPP