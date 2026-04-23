#ifndef COMPLEX_HPP
#define COMPLEX_HPP

#include "../LAB2/Exceptions.hpp"

// Комплексное число
class Complex {
public:
    // Поля
    double re;
    double im;

    // Конструктор
    Complex(double r = 0.0, double i = 0.0) : re(r), im(i) {
    }

    // Арифметические операторы
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
        if (denom == 0.0) {
            throw MathError("Division by zero in complex numbers");
        }
        return Complex((re * other.re + im * other.im) / denom,
                       (im * other.re - re * other.im) / denom);
    }

    // Операторы сравнения с учетом машинной точности
    bool operator==(const Complex& other) const {
        const double EPSILON = 1e-9;

        double diffRe = re - other.re;
        double diffIm = im - other.im;

        if (diffRe < 0.0) {
            diffRe = -diffRe;
        }
        if (diffIm < 0.0) {
            diffIm = -diffIm;
        }

        return (diffRe < EPSILON) && (diffIm < EPSILON);
    }

    bool operator!=(const Complex& other) const {
        return !(*this == other);
    }
};

#endif // COMPLEX_HPP