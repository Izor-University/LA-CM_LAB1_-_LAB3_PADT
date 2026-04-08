#ifndef RECTANGULAR_MATRIX_HPP
#define RECTANGULAR_MATRIX_HPP

#include "IMatrix.hpp"
#include "ArraySequence.hpp"
#include "MutableArraySequence.hpp"
#include "Exceptions.hpp"

template <class T>
class RectangularMatrix : public IMatrix<T> {
private:
    ArraySequence<T>* data;
    int rows;
    int cols;

    int Get1DIndex(int row, int col) const {
        return row * cols + col;
    }
    T Abs(const T& val) const {
        return val < T() ? -val : val;
    }

public:
    RectangularMatrix(int rows, int cols);
    RectangularMatrix(int rows, int cols, const T& defaultValue);
    RectangularMatrix(const RectangularMatrix<T>& other);
    ~RectangularMatrix() override;

    int GetRows() const override;
    int GetCols() const override;
    const T& Get(int row, int col) const override;
    void Set(int row, int col, const T& value) override;

    RectangularMatrix<T>* Add(const RectangularMatrix<T>& other) const;
    RectangularMatrix<T>* MultiplyScalar(const T& scalar) const;
    T Norm() const;

    void SwapRows(int row1, int row2);
    void AddRowMultiple(int sourceRow, int targetRow, const T& factor);
    void MultiplyRow(int row, const T& factor);
};

#include "RectangularMatrix.tpp"
#endif // RECTANGULAR_MATRIX_HPP