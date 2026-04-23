#ifndef SQUARE_MATRIX_HPP
#define SQUARE_MATRIX_HPP

#include "Matrix.hpp"
#include "Vector.hpp"
#include "../LAB2/Exceptions.hpp"

// Квадратная матрица
template <class T>
class SquareMatrix : public Matrix<T> {
public:
    // Конструкторы
    explicit SquareMatrix(int n) : Matrix<T>(n, n) {
    }

    SquareMatrix(int n, const T& defaultValue) : Matrix<T>(n, n, defaultValue) {
    }

    // Жизненный цикл (Rule of Zero)
    virtual ~SquareMatrix() override = default;
    SquareMatrix(const SquareMatrix<T>& other) = default;
    SquareMatrix(SquareMatrix<T>&& other) noexcept = default;
    SquareMatrix<T>& operator=(const SquareMatrix<T>& other) = default;
    SquareMatrix<T>& operator=(SquareMatrix<T>&& other) noexcept = default;

    // Втягивание базовых операторов
    using Matrix<T>::operator*;
    using Matrix<T>::operator+;

    // Математика квадратных матриц
    Vector<T> operator*(const Vector<T>& x) const;
    SquareMatrix<T> operator*(const SquareMatrix<T>& other) const;
    T Trace() const;

    // Фабричный метод
    static SquareMatrix<T> Identity(int n);
};

#include "SquareMatrix.tpp"
#endif // SQUARE_MATRIX_HPP