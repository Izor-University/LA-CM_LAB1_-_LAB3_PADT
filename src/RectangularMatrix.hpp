#ifndef RECTANGULAR_MATRIX_HPP
#define RECTANGULAR_MATRIX_HPP
#include "IMatrix.hpp"
#include "MutableArraySequence.hpp"
#include "Exceptions.hpp"
#include "MathUtils.hpp"

template <class T>
class RectangularMatrix : public IMatrix<T> {
private:
    MutableArraySequence<T>* data;
    int rows;
    int cols;
    int Get1DIndex(int row, int col) const { return row * cols + col; }

public:
    RectangularMatrix(int rows, int cols);
    RectangularMatrix(int rows, int cols, const T& defaultValue);
    RectangularMatrix(const RectangularMatrix<T>& other);      // Copy
    RectangularMatrix(RectangularMatrix<T>&& other) noexcept;  // Move
    ~RectangularMatrix() override;

    RectangularMatrix<T>& operator=(const RectangularMatrix<T>& other);
    RectangularMatrix<T>& operator=(RectangularMatrix<T>&& other) noexcept;

    int GetRows() const override { return rows; }
    int GetCols() const override { return cols; }

    // Поддержка старого интерфейса
    const T& Get(int row, int col) const override { return (*this)(row, col); }
    void Set(int row, int col, const T& value) override { (*this)(row, col) = value; }

    // --- C++ Идиоматичный доступ ---
    T& operator()(int row, int col);
    const T& operator()(int row, int col) const;

    RectangularMatrix<T> operator+(const RectangularMatrix<T>& other) const;
    RectangularMatrix<T> operator*(const T& scalar) const;

    double Norm() const;
    void SwapRows(int row1, int row2);
    void AddRowMultiple(int sourceRow, int targetRow, const T& factor);
    void MultiplyRow(int row, const T& factor);
};
#include "RectangularMatrix.tpp"
#endif