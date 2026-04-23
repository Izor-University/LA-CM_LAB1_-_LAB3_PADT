#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "IMatrix.hpp"
#include "../LAB2/MutableArraySequence.hpp"
#include "../LAB2/Exceptions.hpp"
#include "MathUtils.hpp"

// Прямоугольная матрица
template <class T>
class Matrix : public IMatrix<T> {
protected:
    // Поля
    MutableArraySequence<T>* data;
    int rows;
    int cols;

    // Преобразование 2D индекса в 1D
    int Get1DIndex(int row, int col) const {
        return row * cols + col;
    }

public:
    // Конструкторы и деструктор
    Matrix(int rows, int cols);
    Matrix(int rows, int cols, const T& defaultValue);
    Matrix(const Matrix<T>& other);
    Matrix(Matrix<T>&& other) noexcept;
    virtual ~Matrix() override;

    // Операторы присваивания
    Matrix<T>& operator=(const Matrix<T>& other);
    Matrix<T>& operator=(Matrix<T>&& other) noexcept;

    // Декомпозиция
    int GetRows() const override {
        return rows;
    }
    int GetCols() const override {
        return cols;
    }

    // Доступ к элементам
    const T& Get(int row, int col) const override {
        return (*this)(row, col);
    }
    void Set(int row, int col, const T& value) override {
        (*this)(row, col) = value;
    }
    T& operator()(int row, int col);
    const T& operator()(int row, int col) const;

    // Алгебраические операции
    Matrix<T> operator+(const Matrix<T>& other) const;
    Matrix<T> operator*(const T& scalar) const;
    double Norm() const;

    // Линейные операции над строками
    void SwapRows(int row1, int row2);
    void AddRowMultiple(int sourceRow, int targetRow, const T& factor);
    void MultiplyRow(int row, const T& factor);
};

#include "Matrix.tpp"
#endif // MATRIX_HPP