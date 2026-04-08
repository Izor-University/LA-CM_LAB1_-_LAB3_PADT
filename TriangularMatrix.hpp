#ifndef TRIANGULAR_MATRIX_HPP
#define TRIANGULAR_MATRIX_HPP

#include "IMatrix.hpp"
#include "ArraySequence.hpp"
#include "MutableArraySequence.hpp"
#include "Exceptions.hpp"

template <class T>
class TriangularMatrix : public IMatrix<T> {
private:
    ArraySequence<T>* data; // Используем базовый класс для хранения
    int size;
    bool isLower;
    T zeroElement;

    int Get1DIndex(int row, int col) const;

public:
    TriangularMatrix(int n, bool lower);
    ~TriangularMatrix() override;

    int GetRows() const override;
    int GetCols() const override;
    bool IsLower() const;

    const T& Get(int row, int col) const override;
    void Set(int row, int col, const T& value) override;
};

#include "TriangularMatrix.tpp"
#endif // TRIANGULAR_MATRIX_HPP