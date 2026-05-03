#ifndef SQUARE_MATRIX_HPP
#define SQUARE_MATRIX_HPP

#include "Matrix.hpp"
#include "Vector.hpp"
#include "Exceptions.hpp"

// Квадратная матрица
template <class T>
class SquareMatrix : public Matrix<T> {
public:
    explicit SquareMatrix(int n) : Matrix<T>(n, n) {}
    SquareMatrix(int n, const T& defaultValue) : Matrix<T>(n, n, defaultValue) {}

    virtual ~SquareMatrix() override = default;
    SquareMatrix(const SquareMatrix<T>& other) = default;
    SquareMatrix(SquareMatrix<T>&& other) noexcept = default;
    SquareMatrix<T>& operator=(const SquareMatrix<T>& other) = default;
    SquareMatrix<T>& operator=(SquareMatrix<T>&& other) noexcept = default;

    // Втягиваем базовые операторы, чтобы не потерять Matrix * IMatrix -> Matrix
    using Matrix<T>::operator*;

    // Специфические составные присваивания для квадратной матрицы
    SquareMatrix<T>& operator+=(const IMatrix<T>& other);
    SquareMatrix<T>& operator-=(const IMatrix<T>& other);
    SquareMatrix<T>& operator*=(const T& scalar);
    SquareMatrix<T>& operator*=(const SquareMatrix<T>& other);

    // Специфические арифметические операторы (Square + IMatrix = Square)
    SquareMatrix<T> operator+(const IMatrix<T>& other) const;
    SquareMatrix<T> operator-(const IMatrix<T>& other) const;
    SquareMatrix<T> operator*(const T& scalar) const;
    SquareMatrix<T> operator*(const SquareMatrix<T>& other) const; // Square * Square = Square

    T Trace() const;
    static SquareMatrix<T> Identity(int n);
};

#include "SquareMatrix.tpp"
#endif // SQUARE_MATRIX_HPP