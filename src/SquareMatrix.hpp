#ifndef SQUARE_MATRIX_HPP
#define SQUARE_MATRIX_HPP
#include "IMatrix.hpp"
#include "MutableArraySequence.hpp"
#include "Exceptions.hpp"
#include "MathUtils.hpp"
#include "Vector.hpp"

template <class T>
class SquareMatrix : public IMatrix<T> {
private:
    MutableArraySequence<T>* data;
    int size;
    int Get1DIndex(int row, int col) const { return row * size + col; }

public:
    explicit SquareMatrix(int n);
    SquareMatrix(int n, const T& defaultValue);
    SquareMatrix(const SquareMatrix<T>& other);
    SquareMatrix(SquareMatrix<T>&& other) noexcept;
    ~SquareMatrix() override;

    SquareMatrix<T>& operator=(const SquareMatrix<T>& other);
    SquareMatrix<T>& operator=(SquareMatrix<T>&& other) noexcept;

    int GetRows() const override;
    int GetCols() const override;

    // Legacy support for IMatrix
    const T& Get(int row, int col) const override { return (*this)(row, col); }
    void Set(int row, int col, const T& value) override { (*this)(row, col) = value; }

    // --- C++ Idiomatic Access ---
    T& operator()(int row, int col);
    const T& operator()(int row, int col) const;

    SquareMatrix<T> operator+(const SquareMatrix<T>& other) const;
    SquareMatrix<T> operator*(const T& scalar) const;
    Vector<T> operator*(const Vector<T>& x) const; // Матрица на вектор

    double Norm() const;
    void SwapRows(int row1, int row2);
};
#include "SquareMatrix.tpp"
#endif