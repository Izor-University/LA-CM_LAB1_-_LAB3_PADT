#ifndef SQUARE_MATRIX_HPP
#define SQUARE_MATRIX_HPP

#include "Matrix.hpp"
#include "Vector.hpp"
#include "Exceptions.hpp"

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

    // 1. Умножение на вектор
    Vector<T> operator*(const Vector<T>& x) const;

    // 2. Умножение матрицы на матрицу
    SquareMatrix<T> operator*(const SquareMatrix<T>& other) const;

    // 3. След матрицы (Сумма элементов главной диагонали)
    T Trace() const;

    // 4. Статический фабричный метод: Создание единичной матрицы
    static SquareMatrix<T> Identity(int n);
};

#include "SquareMatrix.tpp"
#endif // SQUARE_MATRIX_HPP