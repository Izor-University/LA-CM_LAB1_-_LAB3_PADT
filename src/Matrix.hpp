#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "IMatrix.hpp"
#include "MutableArraySequence.hpp"
#include "Exceptions.hpp"
#include "MathUtils.hpp"

template <class T>
class Matrix : public IMatrix<T> {
protected:
    MutableArraySequence<T>* data;
    int rows;
    int cols;
    int Get1DIndex(int row, int col) const { return row * cols + col; }

public:
    Matrix(int rows, int cols);
    Matrix(int rows, int cols, const T& defaultValue);
    Matrix(const Matrix<T>& other);
    Matrix(Matrix<T>&& other) noexcept;
    virtual ~Matrix() override;

    Matrix<T>& operator=(const Matrix<T>& other);
    Matrix<T>& operator=(Matrix<T>&& other) noexcept;

    int GetRows() const override { return rows; }
    int GetCols() const override { return cols; }

    const T& Get(int row, int col) const override { return (*this)(row, col); }
    void Set(int row, int col, const T& value) override { (*this)(row, col) = value; }

    T& operator()(int row, int col);
    const T& operator()(int row, int col) const;

    // Вся математика остается здесь!
    Matrix<T> operator+(const Matrix<T>& other) const;
    Matrix<T> operator*(const T& scalar) const;

    double Norm() const;
    void SwapRows(int row1, int row2);
    void AddRowMultiple(int sourceRow, int targetRow, const T& factor);
    void MultiplyRow(int row, const T& factor);
};
#include "Matrix.tpp"
#endif // MATRIX_HPP