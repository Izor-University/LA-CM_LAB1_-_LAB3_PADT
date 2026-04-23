#ifndef DIAGONAL_MATRIX_HPP
#define DIAGONAL_MATRIX_HPP

#include "IMatrix.hpp"
#include "../LAB2/MutableArraySequence.hpp"
#include "../LAB2/Exceptions.hpp"
#include "MathUtils.hpp"

// Диагональная матрица
template <class T>
class DiagonalMatrix : public IMatrix<T> {
private:
    // Поля
    MutableArraySequence<T>* data;
    int size;
    T zeroElement;

public:
    // Конструкторы и деструктор
    explicit DiagonalMatrix(int n);
    DiagonalMatrix(int n, const T& defaultValue);
    DiagonalMatrix(const DiagonalMatrix<T>& other);
    DiagonalMatrix(DiagonalMatrix<T>&& other) noexcept;
    ~DiagonalMatrix() override;

    // Операторы присваивания
    DiagonalMatrix<T>& operator=(const DiagonalMatrix<T>& other);
    DiagonalMatrix<T>& operator=(DiagonalMatrix<T>&& other) noexcept;

    // Декомпозиция
    int GetRows() const override {
        return size;
    }
    int GetCols() const override {
        return size;
    }

    // Доступ к элементам
    const T& Get(int row, int col) const override {
        return (*this)(row, col);
    }
    void Set(int row, int col, const T& value) override;
    T& operator()(int row, int col);
    const T& operator()(int row, int col) const;

    // Алгебраические операции
    DiagonalMatrix<T> operator+(const DiagonalMatrix<T>& other) const;
    DiagonalMatrix<T> operator*(const T& scalar) const;
    double Norm() const;
};

#include "DiagonalMatrix.tpp"
#endif // DIAGONAL_MATRIX_HPP