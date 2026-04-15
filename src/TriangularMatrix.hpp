#ifndef TRIANGULAR_MATRIX_HPP
#define TRIANGULAR_MATRIX_HPP
#include "IMatrix.hpp"
#include "MutableArraySequence.hpp"
#include "Exceptions.hpp"
#include "MathUtils.hpp"

template <class T>
class TriangularMatrix : public IMatrix<T> {
private:
    MutableArraySequence<T>* data;
    int size;
    bool isLower;
    T zeroElement;
    int Get1DIndex(int row, int col) const;

public:
    TriangularMatrix(int n, bool lower);
    TriangularMatrix(const TriangularMatrix<T>& other);
    TriangularMatrix(TriangularMatrix<T>&& other) noexcept;
    ~TriangularMatrix() override;

    TriangularMatrix<T>& operator=(const TriangularMatrix<T>& other);
    TriangularMatrix<T>& operator=(TriangularMatrix<T>&& other) noexcept;

    int GetRows() const override { return size; }
    int GetCols() const override { return size; }
    bool IsLower() const { return isLower; }

    const T& Get(int row, int col) const override { return (*this)(row, col); }
    void Set(int row, int col, const T& value) override;

    T& operator()(int row, int col);
    const T& operator()(int row, int col) const;
};
#include "TriangularMatrix.tpp"
#endif