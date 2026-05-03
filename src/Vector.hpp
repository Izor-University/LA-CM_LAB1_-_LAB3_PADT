#ifndef VECTOR_HPP
#define VECTOR_HPP

#include "MutableArraySequence.hpp"
#include "Exceptions.hpp"
#include "MathUtils.hpp"

// Вектор
template <class T>
class Vector {
private:
    // Поля
    MutableArraySequence<T>* data;
    int size;

public:
    // Конструкторы и деструктор
    explicit Vector(int size);
    Vector(int size, const T& defaultValue);
    Vector(const Vector<T>& other);
    Vector(Vector<T>&& other) noexcept;
    ~Vector();

    // Операторы присваивания
    Vector<T>& operator=(const Vector<T>& other);
    Vector<T>& operator=(Vector<T>&& other) noexcept;

    // Декомпозиция
    int GetSize() const;
    T& operator[](int index);
    const T& operator[](int index) const;

    // Операторы составного присваивания
    Vector<T>& operator+=(const Vector<T>& other);
    Vector<T>& operator-=(const Vector<T>& other);
    Vector<T>& operator*=(const T& scalar);

    // Арифметические операторы
    Vector<T> operator+(const Vector<T>& other) const;
    Vector<T> operator-(const Vector<T>& other) const;
    Vector<T> operator*(const T& scalar) const;
    T operator*(const Vector<T>& other) const;

    // Вычисление нормы
    double Norm() const;
};

#include "Vector.tpp"
#endif // VECTOR_HPP