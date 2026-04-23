#ifndef TRIANGULAR_MATRIX_HPP
#define TRIANGULAR_MATRIX_HPP

#include "IMatrix.hpp"
#include "../LAB2/MutableArraySequence.hpp"
#include "../LAB2/Exceptions.hpp"
#include "MathUtils.hpp"

// Треугольная матрица
template <class T>
class TriangularMatrix : public IMatrix<T> {
private:
    // Поля
    MutableArraySequence<T>* data;
    int size;
    bool isLower;
    T zeroElement;

    // Внутренние методы
    int Get1DIndex(int row, int col) const;

public:
    // Конструкторы и деструктор
    TriangularMatrix(int n, bool lower);
    TriangularMatrix(const TriangularMatrix<T>& other);
    TriangularMatrix(TriangularMatrix<T>&& other) noexcept;
    ~TriangularMatrix() override;

    // Операторы присваивания
    TriangularMatrix<T>& operator=(const TriangularMatrix<T>& other);
    TriangularMatrix<T>& operator=(TriangularMatrix<T>&& other) noexcept;

    // Декомпозиция
    int GetRows() const override {
        return size;
    }
    int GetCols() const override {
        return size;
    }
    bool IsLower() const {
        return isLower;
    }

    // Доступ к элементам
    const T& Get(int row, int col) const override {
        return (*this)(row, col);
    }
    void Set(int row, int col, const T& value) override;

    T& operator()(int row, int col);
    const T& operator()(int row, int col) const;
};

#include "TriangularMatrix.tpp"
#endif // TRIANGULAR_MATRIX_HPP