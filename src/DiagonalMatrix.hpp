#ifndef DIAGONAL_MATRIX_HPP
#define DIAGONAL_MATRIX_HPP
#include "IMatrix.hpp"
#include "MutableArraySequence.hpp"
#include "Exceptions.hpp"
#include "MathUtils.hpp"

template <class T>
class DiagonalMatrix : public IMatrix<T> {
private:
    MutableArraySequence<T>* data;
    int size;
    T zeroElement;

public:
    explicit DiagonalMatrix(int n);
    DiagonalMatrix(int n, const T& defaultValue);
    DiagonalMatrix(const DiagonalMatrix<T>& other);
    DiagonalMatrix(DiagonalMatrix<T>&& other) noexcept;
    ~DiagonalMatrix() override;

    DiagonalMatrix<T>& operator=(const DiagonalMatrix<T>& other);
    DiagonalMatrix<T>& operator=(DiagonalMatrix<T>&& other) noexcept;

    int GetRows() const override { return size; }
    int GetCols() const override { return size; }

    const T& Get(int row, int col) const override { return (*this)(row, col); }
    void Set(int row, int col, const T& value) override;

    T& operator()(int row, int col);
    const T& operator()(int row, int col) const;

    DiagonalMatrix<T> operator+(const DiagonalMatrix<T>& other) const;
    DiagonalMatrix<T> operator*(const T& scalar) const;
    double Norm() const;
};
#include "DiagonalMatrix.tpp"
#endif